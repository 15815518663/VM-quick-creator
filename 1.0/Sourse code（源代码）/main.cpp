// VM Quick Creator - 修复且支持 Windows UTF-8 控制台
#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void printTitle(bool isCN) {
    if (isCN) {
        cout << "=== VM Quick Creator ===" << endl;
        cout << "快速创建虚拟机脚本生成器" << endl << endl;
    } else {
        cout << "=== VM Quick Creator ===" << endl;
        cout << "Quick VM script generator" << endl << endl;
    }
}

void generateVBoxScript(const string &name, int memMB, int diskGB) {
    string filename = name + "_vbox.sh";
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "无法创建文件: " << filename << endl;
        return;
    }
    ofs << "#!/bin/sh\n";
    ofs << "VBoxManage createvm --name \"" << name << "\" --register\n";
    ofs << "VBoxManage modifyvm \"" << name << "\" --memory " << memMB << " --acpi on --boot1 dvd\n";
    ofs << "VBoxManage createmedium disk --filename \"" << name << ".vdi\" --size " << (diskGB * 1024) << "\n";
    ofs << "VBoxManage storagectl \"" << name << "\" --name \"SATA Controller\" --add sata --controller IntelAhci\n";
    ofs << "VBoxManage storageattach \"" << name << "\" --storagectl \"SATA Controller\" --port 0 --device 0 --type hdd --medium \"" << name << ".vdi\"\n";
    ofs.close();
    cout << "VirtualBox 脚本已写入: " << filename << endl;
}

void generateVMwareScript(const string &name, int memMB, int diskGB) {
    string filename = name + "_vmware.sh";
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "无法创建文件: " << filename << endl;
        return;
    }
    ofs << "#!/bin/sh\n";
    ofs << "# Sample VMware commands (using vmrun/vmware-vdiskmanager)\n";
    ofs << "vmware-vdiskmanager -c -s " << (diskGB * 1024) << "MB -a lsilogic -t 0 \"" << name << ".vmdk\"\n";
    ofs << "echo \"Create VMX file or use VMware GUI to register the disk.\"\n";
    ofs.close();
    cout << "VMware 脚本已写入: " << filename << endl;
}

int main() {
#ifdef _WIN32
    // 设置控制台为 UTF-8 避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    // 启用 ANSI 转义序列（可选）
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
    }
#endif

    int langChoice = 1;
    bool isCN = true;
    cout << "请选择语言 / Choose language (1=中文, 2=English): ";
    if (!(cin >> langChoice)) return 1;
    isCN = (langChoice == 1);
    cout << endl;
    printTitle(isCN);

    int vmType = 1;
    if (isCN) {
        cout << "选择虚拟机类型 (1=VirtualBox, 2=VMware): ";
    } else {
        cout << "Select VM type (1=VirtualBox, 2=VMware): ";
    }
    if (!(cin >> vmType)) return 1;

    string vmName;
    if (isCN) {
        cout << "输入虚拟机名称 (无空格): ";
    } else {
        cout << "Enter VM name (no spaces): ";
    }
    if (!(cin >> vmName)) return 1;

    int memMB = 2048;
    if (isCN) {
        cout << "分配内存 (MB)，例如 2048: ";
    } else {
        cout << "Memory (MB), e.g. 2048: ";
    }
    if (!(cin >> memMB)) return 1;

    int diskGB = 20;
    if (isCN) {
        cout << "分配磁盘大小 (GB)，例如 20: ";
    } else {
        cout << "Disk size (GB), e.g. 20: ";
    }
    if (!(cin >> diskGB)) return 1;

    if (vmType == 1) {
        generateVBoxScript(vmName, memMB, diskGB);
    } else {
        generateVMwareScript(vmName, memMB, diskGB);
    }

    if (isCN) {
        cout << "\n配置完成，enjoy！" << endl;
    } else {
        cout << "\nDone, enjoy!" << endl;
    }

    return 0;
}
