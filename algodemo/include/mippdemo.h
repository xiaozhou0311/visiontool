#ifndef ALGODEMO_INCLUDE_MIPPDEMO_H_
#define ALGODEMO_INCLUDE_MIPPDEMO_H_

#include "mipp.h"
#include <string>

namespace mipp_demo {

// MIPP库API示例类
class MippDemo {
 public:
  // 基本加载/存储操作示例
  static void DemoLoadStore();
  
  // 算术运算示例
  static void DemoArithmetic();
  
  // 比较操作示例
  static void DemoComparison();
  
  // 位运算示例
  static void DemoBitwise();
  
  // 掩码操作示例
  static void DemoMasking();
  
  // 数据重排示例
  static void DemoShuffle();
  
  // 归约操作示例
  static void DemoReduction();
  
  // 数学函数示例
  static void DemoMathFunctions();
  
  // 类型转换示例
  static void DemoConversion();
  
  // 对齐和内存操作示例
  static void DemoMemoryAlignment();
  
  // 运行所有示例
  static void RunAllDemos();
  
 private:
  // 辅助函数：打印向量内容
  template<typename T>
  static void PrintRegister(const mipp::reg& r, const std::string& label);
};

}  // namespace mipp_demo

#endif  // ALGODEMO_INCLUDE_MIPPDEMO_H_

