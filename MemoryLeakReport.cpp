#include "MemoryLeakReport.h"
#include <algorithm> 

MemoryLeakReport::MemoryLeakReport(void)
{
}


MemoryLeakReport::~MemoryLeakReport(void)
{
}


int MemoryLeakReport::set_dump_node_from_file(const std::string & file_path)
{
	FILE * file_pointer = fopen(file_path.c_str(), "r");
	if (NULL == file_pointer) {
		return -1;
	}
	
	const int line_max_size = 2048;
	char line_buf[line_max_size] = {0};
	int file_eof = 1;
	while (fgets(line_buf, line_max_size, file_pointer) != NULL && file_eof != EOF)
	{	
		std::string line_buff_str(line_buf);
		// find a dump node
		if (line_buff_str.find("----------") !=  std::string::npos) {
			DumpNode dn; 
			dn.set_data_by_header_block(line_buff_str);
			
			fgets(line_buf, line_max_size, file_pointer);
			line_buff_str = line_buf;
			dn.set_data_by_leakhash_block(line_buff_str);

			fgets(line_buf, line_max_size, file_pointer);

			std::vector<std::string> call_stack;
			do {
				fgets(line_buf, line_max_size, file_pointer);
				line_buff_str = line_buf;

				if (line_buff_str.find("Data:") == 2)
				{
					break;
				}

				call_stack.push_back(line_buff_str);
			} while (true) ;
			dn.set_data_by_callstack_block(call_stack);

			std::vector<std::string> data;
			do {
				if (fgets(line_buf, line_max_size, file_pointer) == NULL) {
					file_eof = EOF;
				}
				line_buff_str = line_buf;

				if (1 == line_buff_str.size() || file_eof == EOF) { break; }

				data.push_back(line_buff_str);
			} while (true) ;
			dn.set_data_by_data_line_block(data);

			dump_node_array.push_back(dn);
		}

		memset(line_buf, 0 , line_max_size);
	}

	fclose(file_pointer);
	file_pointer = NULL;
	return 0;
}

int MemoryLeakReport::write_dump_node_to_file(const std::string & file_path,
											int write_dump_count,
											bool if_write_header, 
											bool if_write_leakhash, 
											bool if_write_callstack,
											bool if_write_data_block)
{
	FILE * file_pointer = fopen(file_path.c_str(), "w+");
	if (NULL == file_pointer) {
		return -1;
	}

	size_t dump_count = 0;
	write_dump_count < 0 ? dump_count = dump_node_array.size() : dump_count = write_dump_count;
	dump_count > dump_node_array.size() ? dump_count = dump_node_array.size() : dump_count;

	for (size_t i = 0; i < dump_count; ++i) {
		dump_node_array[i].write_to_file(file_pointer, if_write_header, 
			         if_write_leakhash, if_write_callstack, if_write_data_block);
	}
	/*for (std::vector<DumpNode>::iterator ite = dump_node_array.begin();
			ite != dump_node_array.end();
			++ite)
	{
		ite->write_to_file(file_pointer);
	}*/

	fclose(file_pointer);
	file_pointer = NULL;
	return 0;
}

int MemoryLeakReport::declining_sort_by_leak_count()
{
	std::sort(dump_node_array.begin(), dump_node_array.end(), DumpNode::declining_compare_by_count);
	return 0;
}

int MemoryLeakReport::declining_sort_by_leak_bytes()
{
	std::sort(dump_node_array.begin(), dump_node_array.end(), DumpNode::declining_compare_by_bytes);
	return 0;
}

int MemoryLeakReport::declining_sort_by_all_leak_bytes()
{
	std::sort(dump_node_array.begin(), dump_node_array.end(), DumpNode::declining_sort_by_all_leak_bytes);
	return 0;
}