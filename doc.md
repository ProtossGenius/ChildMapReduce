# 系统介绍 

- 详情可以查看谷歌的MapReduce论文，这个项目基本是根据该论文写的，以下是简略介绍
- 对大数据的处理主要分为一下步骤
    1. 将输入数据切片，在file\_split.h中有相关的操作
    2. 调用Map方法，将输入转化为Key-Value
    3. 当所有的Mapper都执行完毕，将相同的Key整合到一起（在论文中这个步骤是由Reducer完成的）
    4. 调用Reducer
    5. 每个Reducer会生成一个输出文件

- 简略的步骤可以参考 [MapReduce单元测试](unit_test/mapreduce_test.cc)
