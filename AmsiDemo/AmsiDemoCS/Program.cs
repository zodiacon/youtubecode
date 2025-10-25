using System;
using System.Collections.Generic;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AmsiDemoCS {
    enum AmsiResult {
        CLEAN = 0,
        NOT_DETECTED = 1,
        BLOCKED_BY_ADMIN_START = 0x4000,
        BLOCKED_BY_ADMIN_END = 0x4fff,
        DETECTED = 32768
    }

    static class Amsi {
        [DllImport("amsi.dll", PreserveSig = false)]
        public static extern IntPtr AmsiInitialize([MarshalAs(UnmanagedType.LPWStr)] string name);

        [DllImport("amsi.dll", PreserveSig = false)]
        public static extern IntPtr AmsiOpenSession(IntPtr context);

        [DllImport("amsi.dll", PreserveSig = false)]
        public static extern AmsiResult AmsiScanBuffer(IntPtr context, IntPtr buffer, uint size,
            string content, IntPtr sessoion);

        public static bool IsMalware(AmsiResult result) {
            return result >= AmsiResult.DETECTED;
        }
    }

    internal class Program {

        static void Main(string[] args) {
            if (args.Length == 0) {
                Console.WriteLine("Usage: AmsiDemoCS <file>");
                return;
            }
            try {
                var context = Amsi.AmsiInitialize("Amsi Demo");
                var session = Amsi.AmsiOpenSession(context);

                var mmf = MemoryMappedFile.CreateFromFile(args[0]);
                var acc = mmf.CreateViewAccessor(0, 0);
                unsafe {
                    byte* buffer = null;
                    acc.SafeMemoryMappedViewHandle.AcquirePointer(ref buffer);
                    var result = Amsi.AmsiScanBuffer(context, new IntPtr(buffer), 
                        (uint)new FileInfo(args[0]).Length , "Scan", session);
                    Console.WriteLine($"Malware: {Amsi.IsMalware(result)}");
                }
            }
            catch (Exception e) {
                Console.WriteLine(e.Message);
            }
        }
    }
}
