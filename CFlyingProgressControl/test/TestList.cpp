//
// Created by wang on 18-4-1.
//

#include "TestList.h"

using namespace CList;

std::vector<int> data;
std::ofstream out;
std::ifstream in;
std::string outfile_name;

void testList()
{
    //generateData();
    testAppend();
    testInsert();
}

void testAppend()
{
    in.open("../test/doc/TestListRandomDataIn.txt");
    outfile_name = "../test/doc/TestListAppendOut.txt";
    out.open(outfile_name);
    out.clear();
    if (not out)
    {
        std::cerr<<"open "+outfile_name+" failed." << std::endl;
    }

    List<int> list;
    std::string line;
    while(getline(in, line))
    {
        //std::cout<< line << std::endl;
        int item = atoi(line.c_str());
        list.append(item);
    }
    int index = 0;
    for (auto i = list.begin(); i != list.end(); i = i->succ)
    {
        out << index << "\t " << i->data << std::endl;
        index++;
    }
    out.close();
    in.close();
}
void testInsert()
{
    in.open("../test/doc/TestListAppendOut.txt");
    outfile_name = "../test/doc/TestListInsertOut.txt";
    out.open(outfile_name);
    out.clear();
    if (not out)
    {
        std::cerr<<"open "+outfile_name+" failed." << std::endl;
    }

    List<int> list;
    std::string line;
    std::string str_data;
    while(getline(in, line))
    {
        std::istringstream record(line);
        //get data and abandon meaningless information
        record >> str_data;
        record >> str_data;

        //std::cout<< str_data << std::endl;
        int item = atoi(str_data.c_str());
        list.append(item);
    }
    list.insertAsFirst(999);
    list.insertAsLast(999);
    list.insertAsSucc(2, 200);
    list.insertAsPred(99, 990);
    int index = 0;
    for (auto i = list.begin(); i != list.end(); i = i->succ)
    {
        out << index << '\t' << i->data << std::endl;
        index++;
    }
    out.close();
    in.close();
}
void generateData()
{
    outfile_name = "../test/doc/TestListRandomDataIn.txt";
    out.open(outfile_name);
    if (not out)
    {
        std::cerr<<"open "+outfile_name+" failed." << std::endl;
    }
    for (int i = 0; i < 100; ++i)
    {
        data.push_back(static_cast<int&&>(random() % 100));
        out << *(--data.end()) << std::endl;
    }
    out.close();
}
