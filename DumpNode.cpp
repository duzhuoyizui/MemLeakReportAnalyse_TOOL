#include "DumpNode.h"


DumpNode::DumpNode(void)
	: block_index_(""),
	total_leak_bytes_(0),
	block_addr_(""),
	leak_hash_(""),
	count_(0),
	call_stack_(),
	data_()
{
}

DumpNode::DumpNode(const DumpNode& dn)
	: block_index_(dn.block_index_),
	total_leak_bytes_(dn.total_leak_bytes_),
	block_addr_(dn.block_addr_),
	leak_hash_(dn.leak_hash_),
	count_(dn.count_)
{
	for (std::vector<std::string>::const_iterator ite = dn.call_stack_.begin();
		ite != dn.call_stack_.end(); 
		++ite)
	{
		call_stack_.push_back(*ite);
	}
	
	for (std::vector<std::string>::const_iterator ite = dn.data_.begin();
		ite != dn.data_.end(); 
		++ite)
	{
		data_.push_back(*ite);
	}
}

const DumpNode & DumpNode::operator= (const DumpNode& dn)
{
	if (this == &dn) {
		return *this;
	}

	// clear all data
	block_index_ = dn.block_index_;
	total_leak_bytes_ = dn.total_leak_bytes_;
	block_addr_ = dn.block_addr_;
	leak_hash_ = dn.leak_hash_;
	count_ = dn.count_;

	call_stack_.clear();
	data_.clear();

	for (std::vector<std::string>::const_iterator ite = dn.call_stack_.begin();
		ite != dn.call_stack_.end(); 
		++ite)
	{
		call_stack_.push_back(*ite);
	}
	
	for (std::vector<std::string>::const_iterator ite = dn.data_.begin();
		ite != dn.data_.end(); 
		++ite)
	{
		data_.push_back(*ite);
	}

	return *this;
}
DumpNode::~DumpNode(void)
{
}

int DumpNode::set_data_by_header_block(const std::string & head_block)
{
	// clear all data
	block_index_ = "";
	total_leak_bytes_ = 0;
	block_addr_ = "";

	if (0 == head_block.size()) {
		return -1;
	}
	size_t label = head_block.find("Block");
	size_t temp_label = head_block.find(" at");
	if (label == std::string::npos || temp_label == std::string::npos)
	{
		return -1;
	}
	
	label += sizeof("Block");
	block_index_ = head_block.substr(label, temp_label-label);

	label = temp_label + sizeof(" at");
	temp_label = head_block.find(":");
	block_addr_ = head_block.substr(label, temp_label-label);
	
	label = temp_label + 1;
	temp_label = head_block.find(" bytes");
	std::string str_total_leak_bytes = head_block.substr(label, temp_label-label);
	total_leak_bytes_ = atoi(str_total_leak_bytes.c_str());

	return 0;
}

int DumpNode::set_data_by_leakhash_block(const std::string & leakhash_block)
{
	// clear all data
	leak_hash_ = "";
	count_ = 0;
	
	if (0 == leakhash_block.size()) {
		return -1;
	}

	size_t label = leakhash_block.find("Leak Hash: ");
	size_t temp_label = leakhash_block.find("Count:");
	if (label == std::string::npos || temp_label == std::string::npos) {
		return -1;
	}
	label += sizeof("Leak Hash:");
	leak_hash_ = leakhash_block.substr(label, temp_label-label);
	
	temp_label += sizeof("Count:");
	std::string str_count = leakhash_block.substr(temp_label);
	count_ = atoi(str_count.c_str());

	return 0;
}

int DumpNode::set_data_by_callstack_block(const std::vector<std::string> & callstack_block)
{
	// clear all data
	call_stack_.clear();
	if (0 == callstack_block.size()) {
		return -1;
	}
	
	call_stack_.resize(callstack_block.size());
	copy(callstack_block.begin(), callstack_block.begin()+callstack_block.size(), call_stack_.begin());
	return 0;
}

int DumpNode::set_data_by_data_line_block(const std::vector<std::string> & data_block)
{
	data_.clear();

	if (0 == data_block.size()) {
		return -1;
	}

	data_.resize(data_block.size());

	copy (data_block.begin(), data_block.begin()+data_block.size(), data_.begin());
	
	return 0;
}

int DumpNode::write_to_file(FILE * & file_pointer,
	                bool if_write_header,
					bool if_write_leakhash,
					bool if_write_callstack,
					bool if_write_data_block )
{
	if (if_write_header) {
		fprintf(file_pointer, "---------- Block %s at %s : %d bytes ----------\n", 
			block_index_.c_str(), block_addr_.c_str(), total_leak_bytes_);
	}

	if (if_write_leakhash) {
		fprintf(file_pointer, "Leak Hash: %s Count: %d\n", leak_hash_.c_str(), count_);
	}

	if (if_write_callstack) {
		fprintf(file_pointer, "  Call Stack:\n");
		for (std::vector<std::string>::iterator ite = call_stack_.begin();
			ite != call_stack_.end();
			++ite)
		{
			fprintf(file_pointer, "%s", (*ite).c_str());
		}
	}

	if (if_write_data_block) {
		fprintf(file_pointer, "   Data:\n");
		for (std::vector<std::string>::iterator ite = data_.begin();
			ite != data_.end();
			++ite)
		{
			fprintf(file_pointer, "%s", (*ite).c_str());
		}
	}
	fprintf(file_pointer, "\n\n");
	return 0;
}

bool DumpNode::declining_compare_by_count(const DumpNode& left, const DumpNode& right)
{
	return left.get_count() > right.get_count();
}

bool DumpNode::declining_compare_by_bytes(const DumpNode& left, const DumpNode& right)
{
	return left.get_total_leak_bytes() > right.get_total_leak_bytes();
}

bool DumpNode::declining_sort_by_all_leak_bytes(const DumpNode& left, const DumpNode& right)
{
	return (left.get_count() * left.get_total_leak_bytes()) > (right.get_count() * right.get_total_leak_bytes());
}