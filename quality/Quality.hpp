//
// Created by lzc on 18-7-08..
//
#ifndef Quality_h
#define Quality_h

#include <iostream>
#include <map>
#include <string>

namespace cb
{

    class Quality
    {
    public:
        Quality();
        ~Quality();
        typedef enum
        {
            CollectionTypeLogin,
            CollectionTypeUserDelete,
            CollectionTypeSimpleReply,
            CollectionTypeReply,
            CollectionTypeRecall,
            CollectionTypeFetchMessage,
            CollectionTypeCrash,
            CollectionTypeDatabaseCorruption,
            CollectionTypeDirtyData
        } CollectionType;  //收集类型

        typedef std::map<std::string,int> Map;  //质量收集,这里key用string

        void StatisticsWithCollectionType(CollectionType collection); //主函数
    private:
        void writeFileJson(const char* file);//将信息保存为JSON格式
        void Jsontomap(const char* file, Map&quality_collection);  //将json数据写入map,quality_collection为map
        void maptoJson(const char* file, Map&quality_collection); //将map中数据写回json

        std::string qualitystr[9]=
        {   //用于查找传来的枚举值对应的key
                "CollectionTypeLogin",
                "CollectionTypeUserDelete",
                "CollectionTypeSimpleReply",
                "CollectionTypeReply",
                "CollectionTypeRecall",
                "CollectionTypeFetchMessage",
                "CollectionTypeCrash",
                "CollectionTypeDatabaseCorruption",
                "CollectionTypeDirtyData"
        };

    };

}
#endif /* Quality_h */