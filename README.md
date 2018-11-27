+ Name: MemLeakReportAnalyse_TOOL  
+ Date: 2013.03.20   
+ IDE : Visual Studio 2010 (C++)  
+ Auther: Jerry Zhang  
+ E-Mail: chinajiezhang@gmail.com  
+ Blog: http://www.zhangjiee.com

## Details

MemLeakReportAnalyse_TOOL is a simple tool which can analyse vld。 [virtual leak detector ](http://vld.codeplex.com/) memory_leak_report data.

1. read from file;          
2. declining sort (single leak bytes | leak counts | leak bytes)
3. write to file (Choose your focus data output).

### Remark

1. Output file do not write report header and tailer, just write valid node.
2. Before use this tool, your vld.ini AggregateDuplicates option must be setted "yes";
3. No change the output report file format;
4. Make sure all file path is correct.
