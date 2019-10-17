//import * as sstd from 'std';
//unix c中一切皆文件。FILE* stdin,stdout,stderr;
//https://www.zhihu.com/question/24030687

var file = std.open('std_open_file.js','w');
file.puts('var file = std.open(\"std_open_file.txt\",\"w\");\n');
file.puts('file.puts(\'std_open_file line1\\n\');\n');
file.puts('file.puts(\'std_open_file line2\\n\');\n');
file.puts('file.close();\n');
file.close();
std.loadScript('std_open_file.js');
var rdfile = std.open("std_open_file.txt","r");
do{
    console.log(rdfile.getline());
}while(!rdfile.eof());
rdfile.close();

//1、参考quickjs怎么写curl的
//2、自己写 导出c函数
// -c编写预备 gcc，makefile

