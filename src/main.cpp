#include <stdio.h>

#include <memflow_cpp.h>
#include <memflow_win32_cpp.h>

int main(int argc, char *argv[]) {
	log_init(1);

	CConnectorInventory inv;
	printf("inv: %p\n", inv.inner);

	const char *conn_name = argc > 1? argv[1]: "kvm";
	const char *conn_arg = argc > 2? argv[2]: "";

	CCloneablePhysicalMemory conn = inv.create_connector(conn_name, conn_arg);
	printf("conn: %p\n", conn.inner);

	if (conn) {
		CKernel kernel(conn);
		printf("Kernel: %p\n", kernel.inner);
		Win32Version ver = kernel.winver();
		printf("major: %d\n", ver.nt_major_version);
		printf("minor: %d\n", ver.nt_minor_version);
		printf("build: %d\n", ver.nt_build_number);

		std::vector<CWin32ProcessInfo> process_list = kernel.process_info_vec();

		printf("Process List:\n");
		printf("%-8s | %-16s | %-16s | %-12s | %-5s\n", "PID", "Name", "Base", "DTB", "Wow64");

		for (CWin32ProcessInfo &p : process_list) {
			auto info = p.trait();
			printf("%-8d | %-16s | %-16lx | %-12lx | %-5s\n",
				info.pid(),
				info.name_string().c_str(),
				p.section_base(),
				p.dtb(),
				p.wow64()? "Yes" : "No"
			);
		}
	}

	return 0;
}
