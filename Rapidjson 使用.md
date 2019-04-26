## Rapidjson 使用

[git主页](https://github.com/Tencent/rapidjson.git "点击跳转或直接复制克隆")

[中文文档](http://rapidjson.org/zh-cn/md_doc_dom_8zh-cn.html "点击跳转")

### 导入工程

rapidjson是只依赖于头文件的库，没有动态库、静态库的导入过程。



1. 将头文件加入工程

方法一：将include复制到系统头文件地址，与系统include文件夹合并

方法二：将include复制添加进入工程文件，通常还需设定头文件搜索目录



2. 代码引用头文件

```c
#include "rapidjson/<对应模块>"
```



### 基本使用

#### Document对象

```c++
// Document作为一个JSON的可操作对象
// 1. 将JSON Cstr解析至Document
const char* json = "{\"project\": \"rapidjson}"
Document d;
d.Parse(json);

// 2. 修改Document对象的键值
Value& s = d["stars"];
s.SetInt(s.GetInt() + 1);

// 3. 将Document转换成string
StringBuffer buffer;
Writer<StringBuffer> writer(buffer);
d.Accept(writer);

// 输出 {"project":"rapidjson","stars":11}
std::cout << buffer.GetString() << std::endl;
```

[Document对象的分析](http://rapidjson.org/zh-cn/md_doc_tutorial_8zh-cn.html#ValueDocument "点击跳转")

#### Writer对象

```c++
// 主要作用是将数据分步写入StringBuffer
StringBuffer s;
Writer<StringBuffer> writer(s);

writer.StartObject();               // Between StartObject()/EndObject(), 

writer.Key("hello");                // output a key,
writer.String("world");             // follow by a value.
writer.Key("t");
writer.Bool(true);
writer.Key("f");
writer.Bool(false);
writer.Key("n");
writer.Null();
writer.Key("i");
writer.Uint(123);
writer.Key("pi");
writer.Double(3.1416);
writer.Key("a");
writer.StartArray();                // Between StartArray()/EndArray(),
for (unsigned i = 0; i < 4; i++)
    writer.Uint(i);                 // all values are elements of the array.
writer.EndArray();

writer.EndObject();

// {"hello":"world","t":true,"f":false,"n":null,"i":123,"pi":3.1416,"a":[0,1,2,3]}
cout << s.GetString() << endl;
```

[Writer 成员函数列表](http://rapidjson.org/zh-cn/classrapidjson_1_1_writer.html "点击跳转")

### Reader对象 和 数据Validation

分别见`/example`中的`schemavalidator`和`simplewritter`