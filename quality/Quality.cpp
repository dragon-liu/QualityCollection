//
// Created by lzc on 18-7-08..
//
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdio>
#include <cstdlib>

#include "Quality.hpp"
#include "log/Log.hpp"
#include "vender/rapidjson/include/rapidjson/document.h"
#include "vender/rapidjson/include/rapidjson/prettywriter.h"
#include "vender/rapidjson/include/rapidjson/filewritestream.h"
#include "vender/rapidjson/include/rapidjson/memorybuffer.h"

using namespace std;
using namespace cb;

Quality::Quality()
{

}
Quality::~Quality()
{

}
void Quality::StatisticsWithCollectionType(CollectionType collection)
{
	ifstream json;
	json.open("/sdcard/QualityCollection.json", ios::in);  //判断json文件是否已存在
	if (!json.is_open())
	{
		writeFileJson("/sdcard/QualityCollection.json");  //json文件不存在则初始化json文件
	}
	else
	{
		json.close(); //若已存在json文件则关闭
	}

    //用来读取json文件的map
	Map quality_collection;

    //将json中数据写入map
	Jsontomap("/sdcard/QualityCollection.json", quality_collection);

	//根据传入类型修改map相应数据
    (quality_collection[qualitystr[collection]])++;

	//将修改好的数据再次写入json保存
	maptoJson("/sdcard/QualityCollection.json", quality_collection);
}

//将信息保存为JSON格式
void Quality:: writeFileJson(const char* file)
{
	//生成Json串
    rapidjson::Document jsonDoc;    //生成一个dom元素Document
    rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
    jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素

	//创建模板json文件
	jsonDoc.AddMember("CollectionTypeLogin", 0, allocator);
	jsonDoc.AddMember("CollectionTypeUserDelete", 0, allocator);
	jsonDoc.AddMember("CollectionTypeSimpleReply", 0, allocator);
	jsonDoc.AddMember("CollectionTypeReply", 0, allocator);
	jsonDoc.AddMember("CollectionTypeRecall", 0, allocator);
	jsonDoc.AddMember("CollectionTypeFetchMessage", 0, allocator);
	jsonDoc.AddMember("CollectionTypeCrash", 0, allocator);
	jsonDoc.AddMember("CollectionTypeDatabaseCorruption", 0, allocator);
	jsonDoc.AddMember("CollectionTypeDirtyData", 0, allocator);

	//输出到文件 
	rapidjson::StringBuffer buffer;  
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json  
    jsonDoc.Accept(pretty_writer);

    ofstream fout;  
    fout.open(file);    
    if ( !fout.is_open() ) //判断新建收集json文件是否失败
	{
		SE_LOG_ERROR(kQualityModule, "%s", "open file failed.");
	}
    fout<<buffer.GetString();  
    fout.close(); 

	//检验是否创建成功
	ifstream os;
	os.open(file, ios::in);
	if (!os.is_open()) //判断新建收集json文件是否失败
	{
		SE_LOG_ERROR(kQualityModule, "%s", "create collection_file failed.");
	}
	else
    {
	    SE_LOG_INFO(kQualityModule, "%s", "create collection_file successful.");
		os.close();
	}

	/*
    //json文件内容如下：
    {
          "CollectionTypeLogin":0, allocator,  登录
          "CollectionTypeUserDelete": 0, allocator,  删除用户
          "CollectionTypeSimpleReply": 0, allocator,  快捷回复
          "CollectionTypeReply": 0, allocator,   回复
          "CollectionTypeRecall": 0, allocator,   撤销
          "CollectionTypeFetchMessage": 0, allocator ,  获取邮件
          "CollectionTypeCrash": 0, allocator,    程序崩溃
          "CollectionTypeDatabaseCorruption": 0, allocator,  数据库损坏
          "CollectionTypeDirtyData": 0, allocator   数据库脏数据
    }
    */
}

void Quality::Jsontomap(const char* file, Map&quality_collection)
{
	ifstream fin;  
    fin.open(file, ios::in); 
	if (!fin.is_open()) //判断新建收集json文件是否失败
	{
		SE_LOG_ERROR(kQualityModule, "%s", "open file failed.");
	} 
    string str;  
    string str_in="";  
    while(getline(fin,str))    //一行一行地读到字符串str_in中  
    {  
        str_in=str_in+str+'\n';  
    }  
    //解析并打印出来  
    rapidjson::Document doc;
    doc.Parse<0>(str_in.c_str());  
	//判断读取成功与否 
	if (doc.HasParseError())
	{
		SE_LOG_ERROR(kQualityModule, "%s", "Parse file failed.");
		return;
	}
    //遍历Json文件写入map
	for (auto&m: doc.GetObject())
    {
        //逐个提取数组元素（声明的变量必须为引用)
        quality_collection[m.name.GetString()] = m.value.GetInt();
    }
}

void Quality::maptoJson(const char* file, Map& quality_collection)
{
    rapidjson::Document document;
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value root(rapidjson::kObjectType);
    rapidjson::Value key(rapidjson::kStringType);

    map<string, int>::const_iterator it;
    for (it = quality_collection.begin(); it != quality_collection.end(); ++it)  // 注意这里要用const_iterator
    {
        key.SetString(it->first.c_str(), allocator);
        root.AddMember(key, it->second, allocator);
    }

    //输出到文件
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> pretty_writer(buffer);  //PrettyWriter是格式化的json
    root.Accept(pretty_writer);

    ofstream fout;
    fout.open(file); 
    if (!fout.is_open()) //判断新建收集json文件是否失败
	{
		SE_LOG_ERROR(kQualityModule, "%s", "open file failed.");
	}    
    fout << buffer.GetString();
    fout.close();
    //检验是否创建成功
    ifstream os;
    os.open(file, ios::in);
    if (!os.is_open()) //新建收集json文件失败
    {
        SE_LOG_ERROR(kQualityModule, "%s", "create collection_file failed.");
    } 
    else
    {
        SE_LOG_INFO(kQualityModule, "%s", "create collection_file successful.");
        os.close();
    }
}
	
