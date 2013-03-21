#pragma once

#include <string>
#include <vector>
#include <iostream>

class DumpNode
{
public:
	DumpNode(void);
	~DumpNode(void);
	
	DumpNode(const DumpNode& dn);
	const DumpNode & operator= (const DumpNode& dn);

	int set_data_by_header_block(const std::string & head_block);
	int set_data_by_leakhash_block(const std::string & leakhash_block);
	int set_data_by_callstack_block(const std::vector<std::string> & callstack_block);
	int set_data_by_data_line_block(const std::vector<std::string> & data_block);

	int write_to_file(FILE * & file_pointer,
		            bool if_write_header = true, 
					bool if_write_leakhash = true, 
					bool if_write_callstack = true,
					bool if_write_data_block = true);

	int get_count() const { return count_; }
	int get_total_leak_bytes() const { return total_leak_bytes_; }

	// compare for sort
	static bool declining_compare_by_count(const DumpNode& left, const DumpNode& right);
	static bool declining_compare_by_bytes(const DumpNode& left, const DumpNode& right);
	static bool declining_sort_by_all_leak_bytes(const DumpNode& left, const DumpNode& right);

	//int __debug_output()
	//{
	//	std::cout << "------------»ªÀö·Ö¸îÏß------------" << std::endl;

	//	// header block
	//	std::cout << block_index_ << std::endl;
	//	std::cout << block_addr_ << std::endl;
	//	std::cout << total_leak_bytes_ << std::endl;
	//	
	//	// leakhash block
	//	std::cout << leak_hash_ << std::endl;
	//	std::cout << count_ << std::endl;

	//	for (std::vector<std::string>::iterator ite = call_stack_.begin();
	//		ite != call_stack_.end();
	//		++ite)
	//	{
	//		std::cout << *ite ;
	//	}
	//	
	//	for (std::vector<std::string>::iterator ite = data_.begin();
	//		ite != data_.end();
	//		++ite)
	//	{
	//		std::cout << *ite ;
	//	}

	//	
	//	return 0;
	//}

private:
	std::string              block_index_;
	int                      total_leak_bytes_;
	std::string              block_addr_;

	std::string              leak_hash_;
	int                      count_;

	std::vector<std::string> call_stack_;
	std::vector<std::string> data_;
};

