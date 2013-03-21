#pragma once
#include "DumpNode.h"

class MemoryLeakReport
{
public:
	MemoryLeakReport(void);
	~MemoryLeakReport(void);

	int set_dump_node_from_file(const std::string & file_path);

	// if @write_dump_count < 0, write out all dump node.
	int write_dump_node_to_file(const std::string & file_path,
		                        int write_dump_count = -1,
		                        bool if_write_header = true, 
		                        bool if_write_leakhash = true, 
		                        bool if_write_callstack = true,
								bool if_write_data_block = true);

	int declining_sort_by_leak_count();
	int declining_sort_by_leak_bytes();
	int declining_sort_by_all_leak_bytes(); // count * bytes

	//int __debug_output() 
	//{
	//	for (std::vector<DumpNode>::iterator ite = dump_node_array.begin();
	//		ite != dump_node_array.end();
	//		++ite)
	//	{
	//		ite->__debug_output();
	//	}

	//	return 0;
	//}

private:
	std::vector<DumpNode> dump_node_array;
};

