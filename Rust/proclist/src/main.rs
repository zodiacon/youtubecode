use windows::Win32::System::Diagnostics::ToolHelp::*;

fn main() {
    unsafe {
        let snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0).unwrap();

        let mut pe = PROCESSENTRY32W::default();
        pe.dwSize = size_of::<PROCESSENTRY32W>() as u32;

        Process32FirstW(snapshot, &mut pe).unwrap();

        loop {
            let name = String::from_utf16_lossy(&pe.szExeFile);
            let name = name.trim_end_matches('\0');
            println!("Process ID: {:6}: {:?}", pe.th32ProcessID, name);
            if Process32NextW(snapshot, &mut pe).is_err() { break; }
        }
    }
}
