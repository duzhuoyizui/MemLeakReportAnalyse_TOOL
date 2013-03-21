/* ***********************************************************************************************
 * Name: MemLeakReportAnalyse_TOOL
 * version: 1.0
 * 
 * IDE : Visual Studio 2010 (C++)
 *
 * Auther: Jerry Zhang
 * E-Mail: chinajiezhang@gmail.com
 * Date: 2013.03.20
 * 
 * Details: MemLeakReportAnalyse_TOOL is a simple tool which can analyse vld(virtual leak detector ,http://vld.codeplex.com/) memory_leak_report data.
 *         1. read from file; 
 *         2. declining sort (single leak bytes | leak counts | total leak bytes);
 *         3. write to file (Choose your focus data output).
 * 
 * Remark: 1. Output file do not write report header and tailer, just write valid node.
 *         2. Before use this tool , your vld.ini AggregateDuplicates option must be setted "yes";
 *         3. No change the output report file format;
 *         4. Make sure all file path is correct.
 * ************************************************************************************************/


#include <iostream>
#include "DumpNode.h"
#include "MemoryLeakReport.h"
#include <vld.h>

int main()
{	
	std::string file_path_in = ".\\memory_leak_report.txt";
	std::string file_path_out1 = ".\\declining_sort_by_leak_bytes.txt";
	std::string file_path_out2 = ".\\declining_sort_by_leak_count.txt";
	std::string file_path_out3 = ".\\declining_sort_by_all_leak_bytes.txt";

	MemoryLeakReport mlr;
	if (mlr.set_dump_node_from_file(file_path_in) == -1)
	{
		std::cerr << "invalid file_path_in !" << std::endl;
		return 0;
	}

	mlr.declining_sort_by_leak_bytes();
	if (mlr.write_dump_node_to_file(file_path_out1, -1, true, true, true, false) == -1)
	{
		std::cerr << "invalid file_path_out in write_dump_node_to_file" << std::endl;
		return 0;
	}

	mlr.declining_sort_by_leak_count();
	if (mlr.write_dump_node_to_file(file_path_out2, -1, true, true, true, false) == -1) 
	{
		std::cerr << "invalid file_path_out int write_dump_node_to_file" << std::endl;
		return 0;
	}

	mlr.declining_sort_by_all_leak_bytes();
	if (mlr.write_dump_node_to_file(file_path_out3, -1, true, true, true, false)) {
		std::cerr << "invalid file_path_out in declining_sort_by_all_leak_bytes" << std::endl;
		return 0;
	}

	return 0;
}