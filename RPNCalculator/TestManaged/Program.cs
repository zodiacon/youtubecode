using System;
using System.Runtime.InteropServices;

namespace TestManaged {
    [ComImport, Guid("166978B1-C2DE-4BA6-93CA-D625660DC748")]
    class RPNCalc {
    }


    [Guid("525B283A-BE85-4515-904D-B289CF0BA0A6"),
        InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    interface IRPNCalc {
        void Push(int n);
        int Pop();
        void Add();
        void Sub();
        void Mul();
        void Div();
        bool IsEmpty();
    }

    internal class Program {
        [STAThread]
        static void Main(string[] args) {
            try {
                var calc = (IRPNCalc)new RPNCalc();
                calc.Push(1);
                calc.Push(2);
                calc.Push(3);
                calc.Mul();
                calc.Add();
                int result = calc.Pop();
                Console.WriteLine($"Result {result}");
            }
            catch (COMException ex) {
                Console.WriteLine(ex.Message);
            }
        }
    }
}
