#include "mippdemo.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include <chrono>

namespace mipp_demo {

// 辅助函数：打印向量内容
template<typename T>
void PrintRegister(const mipp::Reg<T>& r, const std::string& label) {
  T values[mipp::N<T>()];
  mipp::store<T>(values, r.r);
  
  std::cout << label << ": [";
  for (int i = 0; i < mipp::N<T>(); i++) {
    std::cout << (i > 0 ? ", " : "") << values[i];
  }
  std::cout << "]" << std::endl;
}

// 基本加载/存储操作
void MippDemo::DemoLoadStore() {
  std::cout << "\n===== 基本加载/存储操作示例 =====\n" << std::endl;
  
  // 准备数据
  const int size = mipp::N<float>();
  float data[size];
  for (int i = 0; i < size; i++) {
    data[i] = i + 1.0f;
  }
  
  // 1. 加载操作 - load (对齐加载)
  std::cout << "1. load - 对齐加载" << std::endl;
  alignas(32) float aligned_data[size];
  for (int i = 0; i < size; i++) {
    aligned_data[i] = i + 1.0f;
  }
  mipp::reg r1 = mipp::load<float>(aligned_data);
  PrintRegister<float>(r1, "对齐加载结果");
  
  // 2. 加载操作 - loadu (非对齐加载)
  std::cout << "\n2. loadu - 非对齐加载" << std::endl;
  mipp::reg r2 = mipp::loadu<float>(data);
  PrintRegister<float>(r2, "非对齐加载结果");
  
  // 3. 存储操作 - store (对齐存储)
  std::cout << "\n3. store - 对齐存储" << std::endl;
  alignas(32) float store_result[size];
  mipp::store<float>(store_result, r1);
  std::cout << "对齐存储结果: [";
  for (int i = 0; i < size; i++) {
    std::cout << (i > 0 ? ", " : "") << store_result[i];
  }
  std::cout << "]" << std::endl;
  
  // 4. 存储操作 - storeu (非对齐存储)
  std::cout << "\n4. storeu - 非对齐存储" << std::endl;
  float storeu_result[size];
  mipp::storeu<float>(storeu_result, r2);
  std::cout << "非对齐存储结果: [";
  for (int i = 0; i < size; i++) {
    std::cout << (i > 0 ? ", " : "") << storeu_result[i];
  }
  std::cout << "]" << std::endl;
  
  // 5. 设置单一值 - set1
  std::cout << "\n5. set1 - 设置单一值" << std::endl;
  mipp::reg r3 = mipp::set1<float>(3.14f);
  PrintRegister<float>(r3, "设置单一值结果");
  
  // 6. 设置多个值 - set
  std::cout << "\n6. set - 设置多个值" << std::endl;
  mipp::reg r4 = mipp::set(data);
  PrintRegister<float>(r4, "设置多个值结果");
}

// 算术运算示例
void MippDemo::DemoArithmetic() {
  std::cout << "\n===== 算术运算示例 =====\n" << std::endl;
  
  // 准备数据
  mipp::reg r1 = mipp::set1<float>(10.0f);
  mipp::reg r2 = mipp::set1<float>(2.0f);
  
  // 1. 加法 - add
  std::cout << "1. add - 加法" << std::endl;
  mipp::reg r_add = mipp::add<float>(r1, r2);
  PrintRegister<float>(r1, "操作数1");
  PrintRegister<float>(r2, "操作数2");
  PrintRegister<float>(r_add, "加法结果");
  
  // 2. 减法 - sub
  std::cout << "\n2. sub - 减法" << std::endl;
  mipp::reg r_sub = mipp::sub<float>(r1, r2);
  PrintRegister<float>(r_sub, "减法结果");
  
  // 3. 乘法 - mul
  std::cout << "\n3. mul - 乘法" << std::endl;
  mipp::reg r_mul = mipp::mul<float>(r1, r2);
  PrintRegister<float>(r_mul, "乘法结果");
  
  // 4. 除法 - div
  std::cout << "\n4. div - 除法" << std::endl;
  mipp::reg r_div = mipp::div<float>(r1, r2);
  PrintRegister<float>(r_div, "除法结果");
  
  // 5. 最小值 - min
  std::cout << "\n5. min - 最小值" << std::endl;
  mipp::reg r_min = mipp::min<float>(r1, r2);
  PrintRegister<float>(r_min, "最小值结果");
  
  // 6. 最大值 - max
  std::cout << "\n6. max - 最大值" << std::endl;
  mipp::reg r_max = mipp::max<float>(r1, r2);
  PrintRegister<float>(r_max, "最大值结果");
  
  // 7. 绝对值 - abs
  std::cout << "\n7. abs - 绝对值" << std::endl;
  mipp::reg r_neg = mipp::set1<float>(-5.0f);
  mipp::reg r_abs = mipp::abs<float>(r_neg);
  PrintRegister<float>(r_neg, "负数");
  PrintRegister<float>(r_abs, "绝对值结果");
  
  // 8. 平方根 - sqrt
  std::cout << "\n8. sqrt - 平方根" << std::endl;
  mipp::reg r_sqrt = mipp::sqrt<float>(r1);
  PrintRegister<float>(r_sqrt, "平方根结果");
  
  // 9. 倒数 - rcp
  std::cout << "\n9. rcp - 倒数" << std::endl;
  mipp::reg r_rcp = mipp::div<float>(mipp::set1<float>(1.0f), r1);
  PrintRegister<float>(r_rcp, "倒数结果");
  
  // 10. 倒数平方根 - rsqrt
  std::cout << "\n10. rsqrt - 倒数平方根" << std::endl;
  mipp::reg r_rsqrt = mipp::rsqrt<float>(r1);
  PrintRegister<float>(r_rsqrt, "倒数平方根结果");
}

// 比较操作示例
void MippDemo::DemoComparison() {
  std::cout << "\n===== 比较操作示例 =====\n" << std::endl;
  
  // 准备数据
  mipp::reg r1 = mipp::set1<float>(10.0f);
  mipp::reg r2 = mipp::set1<float>(2.0f);
  float values[4] = {1.0f, 10.0f, 5.0f, 2.0f};
  mipp::reg r3 = mipp::loadu<float>(values);
  
  // 1. 等于 - cmpeq
  std::cout << "1. cmpeq - 等于" << std::endl;
  mipp::msk m_eq = mipp::cmpeq<float>(r1, r3);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_eq), i) << " ";
  }
  std::cout << std::endl;
  
  // 2. 不等于 - cmpneq
  std::cout << "\n2. cmpneq - 不等于" << std::endl;
  mipp::msk m_neq = mipp::cmpneq<float>(r1, r3);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_neq), i) << " ";
  }
  std::cout << std::endl;
  
  // 3. 小于 - cmplt
  std::cout << "\n3. cmplt - 小于" << std::endl;
  mipp::msk m_lt = mipp::cmplt<float>(r3, r1);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_lt), i) << " ";
  }
  std::cout << std::endl;
  
  // 4. 小于等于 - cmple
  std::cout << "\n4. cmple - 小于等于" << std::endl;
  mipp::msk m_le = mipp::cmple<float>(r3, r1);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_le), i) << " ";
  }
  std::cout << std::endl;
  
  // 5. 大于 - cmpgt
  std::cout << "\n5. cmpgt - 大于" << std::endl;
  mipp::msk m_gt = mipp::cmpgt<float>(r3, r2);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_gt), i) << " ";
  }
  std::cout << std::endl;
  
  // 6. 大于等于 - cmpge
  std::cout << "\n6. cmpge - 大于等于" << std::endl;
  mipp::msk m_ge = mipp::cmpge<float>(r3, r2);
  std::cout << "掩码结果: ";
  for (int i = 0; i < mipp::N<float>(); i++) {
    std::cout << mipp::get<float>(mipp::toreg<mipp::N<float>()>(m_ge), i) << " ";
  }
  std::cout << std::endl;
}

// 位运算示例
void MippDemo::DemoBitwise() {
  std::cout << "\n===== 位运算示例 =====\n" << std::endl;
  
  // 准备数据
  mipp::reg r1 = mipp::set1<int32_t>(0x0F0F0F0F);  // 00001111 00001111 ...
  mipp::reg r2 = mipp::set1<int32_t>(0xF0F0F0F0);  // 11110000 11110000 ...
  
  // 1. 按位与 - andb
  std::cout << "1. andb - 按位与" << std::endl;
  mipp::reg r_and = mipp::andb<int32_t>(r1, r2);
  PrintRegister<int32_t>(r1, "操作数1 (十六进制)");
  PrintRegister<int32_t>(r2, "操作数2 (十六进制)");
  PrintRegister<int32_t>(r_and, "按位与结果 (十六进制)");
  
  // 2. 按位或 - orb
  std::cout << "\n2. orb - 按位或" << std::endl;
  mipp::reg r_or = mipp::orb<int32_t>(r1, r2);
  PrintRegister<int32_t>(r_or, "按位或结果 (十六进制)");
  
  // 3. 按位异或 - xorb
  std::cout << "\n3. xorb - 按位异或" << std::endl;
  mipp::reg r_xor = mipp::xorb<int32_t>(r1, r2);
  PrintRegister<int32_t>(r_xor, "按位异或结果 (十六进制)");
  
  // 4. 按位非 - notb
  std::cout << "\n4. notb - 按位非" << std::endl;
  mipp::reg r_not = mipp::notb<int32_t>(r1);
  PrintRegister<int32_t>(r_not, "按位非结果 (十六进制)");
  
  // 5. 左移 - lshift
  std::cout << "\n5. lshift - 左移" << std::endl;
  mipp::reg r_lshift = mipp::lshift<int32_t>(r1, 4);
  PrintRegister<int32_t>(r_lshift, "左移4位结果 (十六进制)");
  
  // 6. 右移 - rshift
  std::cout << "\n6. rshift - 右移" << std::endl;
  mipp::reg r_rshift = mipp::rshift<int32_t>(r1, 4);
  PrintRegister<int32_t>(r_rshift, "右移4位结果 (十六进制)");
}

// 掩码操作示例
void MippDemo::DemoMasking() {
  std::cout << "\n===== 掩码操作示例 =====\n" << std::endl;
  
  // 准备数据
  const int size = mipp::N<float>();
  float data1[size], data2[size];
  for (int i = 0; i < size; i++) {
    data1[i] = i + 1.0f;
    data2[i] = (i + 1.0f) * 10.0f;
  }
  
  mipp::reg r1 = mipp::loadu<float>(data1);
  mipp::reg r2 = mipp::loadu<float>(data2);
  
  // 创建掩码
  bool mask_values[size];
  for (int i = 0; i < size; i++) {
    mask_values[i] = (i % 2 == 0);  // 偶数索引为true
  }
  mipp::msk m = mipp::set<size>(mask_values);
  
  // 1. blend - 混合
  std::cout << "1. blend - 混合" << std::endl;
  PrintRegister<float>(r1, "向量1");
  PrintRegister<float>(r2, "向量2");
  std::cout << "掩码 (1=true, 0=false): ";
  for (int i = 0; i < size; i++) {
    std::cout << mask_values[i] << " ";
  }
  std::cout << std::endl;
  
  mipp::reg r_blend = mipp::blend<float>(r1, r2, m);
  PrintRegister<float>(r_blend, "混合结果");
  
  // 2. maskzld - 掩码加载
  std::cout << "\n2. maskzld - 掩码加载" << std::endl;
  mipp::reg r_maskzld = mipp::maskzld<float>(m, data2);
  PrintRegister<float>(r_maskzld, "掩码加载结果");
  
  // 3. maskst - 掩码存储
  std::cout << "\n3. maskst - 掩码存储" << std::endl;
  float maskst_result[size];
  for (int i = 0; i < size; i++) {
    maskst_result[i] = 0.0f;  // 初始化为0
  }
  mipp::maskst<float>(m, maskst_result, r1);
  std::cout << "掩码存储结果: [";
  for (int i = 0; i < size; i++) {
    std::cout << (i > 0 ? ", " : "") << maskst_result[i];
  }
  std::cout << "]" << std::endl;
  
  // 4. mask - 掩码操作
  std::cout << "\n4. mask - 掩码操作 (add)" << std::endl;
  mipp::reg r_mask_add = mipp::mask<float, mipp::add<float>>(m, r1, r1, r2);
  PrintRegister<float>(r_mask_add, "掩码加法结果");
  
  // 5. maskz - 掩码操作 (结果为0)
  std::cout << "\n5. maskz - 掩码操作 (mul, 结果为0)" << std::endl;
  mipp::reg r_maskz_mul = mipp::maskz<float, mipp::mul<float>>(m, r1, r2);
  PrintRegister<float>(r_maskz_mul, "掩码乘法结果");
}

// 数据重排示例
void MippDemo::DemoShuffle() {
  std::cout << "\n===== 数据重排示例 =====\n" << std::endl;
  
  // 准备数据
  const int size = mipp::N<float>();
  float data[size];
  for (int i = 0; i < size; i++) {
    data[i] = i + 1.0f;
  }
  mipp::reg r = mipp::loadu<float>(data);
  
  // 1. interleave - 交错
  std::cout << "1. interleave - 交错" << std::endl;
  PrintRegister<float>(r, "原始数据");
  
  float data_in[mipp::N<float>()];
  mipp::store<float>(data_in, r);
  float data_low[mipp::N<float>()], data_high[mipp::N<float>()];

  for (int i = 0; i < mipp::N<float>()/2; i++) {
    data_low[2*i]   = data_in[i];
    data_low[2*i+1] = data_in[i];
    data_high[2*i]  = data_in[i + mipp::N<float>()/2];
    data_high[2*i+1]= data_in[i + mipp::N<float>()/2];
  }

  mipp::reg r_low = mipp::loadu<float>(data_low);
  mipp::reg r_high = mipp::loadu<float>(data_high);
  
  // 2. deinterleave - 解交错
  //std::cout << "\n2. deinterleave - 解交错" << std::endl;
  //mipp::reg r_even, r_odd;
  //mipp::deinterleave<float>(r_low, r_high, r_even, r_odd);
  //PrintRegister<float>(r_even, "解交错偶数结果");
  //PrintRegister<float>(r_odd, "解交错奇数结果");
  
  // 3. interleavelo/interleavehi - 低位/高位交错
  std::cout << "\n3. interleavelo/interleavehi - 低位/高位交错" << std::endl;
  mipp::reg r_interleavelo = mipp::interleavelo<float>(r, r);
  mipp::reg r_interleavehi = mipp::interleavehi<float>(r, r);
  PrintRegister<float>(r_interleavelo, "低位交错结果");
  PrintRegister<float>(r_interleavehi, "高位交错结果");
  
  // 4. transpose - 转置
  if (size >= 4) {
    std::cout << "\n4. transpose - 转置" << std::endl;
    mipp::reg r_rows[4];
    for (int i = 0; i < 4; i++) {
      float row_data[size];
      for (int j = 0; j < size; j++) {
        row_data[j] = i * size + j + 1.0f;
      }
      r_rows[i] = mipp::loadu<float>(row_data);
      std::cout << "行 " << i << ": ";
      PrintRegister<float>(r_rows[i], "");
    }
    
    mipp::transpose<float>(r_rows);
    
    std::cout << "转置后:" << std::endl;
    for (int i = 0; i < 4; i++) {
      std::cout << "行 " << i << ": ";
      PrintRegister<float>(r_rows[i], "");
    }
  }
}

// 归约操作示例
void MippDemo::DemoReduction() {
  std::cout << "\n===== 归约操作示例 =====\n" << std::endl;
  
  // 准备数据
  const int size = mipp::N<float>();
  float data[size];
  for (int i = 0; i < size; i++) {
    data[i] = i + 1.0f;
  }
  mipp::reg r = mipp::loadu<float>(data);
  
  // 1. sum - 求和
  std::cout << "1. sum - 求和" << std::endl;
  PrintRegister<float>(r, "原始数据");
  float sum = mipp::sum<float>(r);
  std::cout << "求和结果: " << sum << std::endl;
  
  // 2. hadd - 水平加法
  std::cout << "\n2. hadd - 水平加法" << std::endl;
  float hadd_result = mipp::hadd<float>(r);
  std::cout << "水平加法结果: " << hadd_result << std::endl;
  
  // 3. hmul - 水平乘法
  std::cout << "\n3. hmul - 水平乘法" << std::endl;
  float hmul_result = mipp::hmul<float>(r);
  std::cout << "水平乘法结果: " << hmul_result << std::endl;
  
  // 4. hmin - 水平最小值
  std::cout << "\n4. hmin - 水平最小值" << std::endl;
  float hmin_result = mipp::hmin<float>(r);
  std::cout << "水平最小值结果: " << hmin_result << std::endl;
  
  // 5. hmax - 水平最大值
  std::cout << "\n5. hmax - 水平最大值" << std::endl;
  float hmax_result = mipp::hmax<float>(r);
  std::cout << "水平最大值结果: " << hmax_result << std::endl;
}

// 数学函数示例
void MippDemo::DemoMathFunctions() {
  std::cout << "\n===== 数学函数示例 =====\n" << std::endl;
  
  // 准备数据
  mipp::reg r1 = mipp::set1<float>(0.5f);
  mipp::reg r2 = mipp::set1<float>(2.0f);
  
  // 1. sin - 正弦
  std::cout << "1. sin - 正弦" << std::endl;
  PrintRegister<float>(r1, "输入值");
  mipp::reg r_sin = mipp::sin<float>(r1);
  PrintRegister<float>(r_sin, "正弦结果");
  
  // 2. cos - 余弦
  std::cout << "\n2. cos - 余弦" << std::endl;
  mipp::reg r_cos = mipp::cos<float>(r1);
  PrintRegister<float>(r_cos, "余弦结果");
  
  // 3. tan - 正切
  std::cout << "\n3. tan - 正切" << std::endl;
  mipp::reg r_tan = mipp::tan<float>(r1);
  PrintRegister<float>(r_tan, "正切结果");
  
  // 4. exp - 指数
  std::cout << "\n4. exp - 指数" << std::endl;
  mipp::reg r_exp = mipp::exp<float>(r1);
  PrintRegister<float>(r_exp, "指数结果");
  
  // 5. log - 对数
  std::cout << "\n5. log - 对数" << std::endl;
  mipp::reg r_log = mipp::log<float>(r2);
  PrintRegister<float>(r_log, "对数结果");
  
  // 6. pow - 幂
  std::cout << "\n6. pow - 幂" << std::endl;
  float base[mipp::N<float>()], exponent[mipp::N<float>()], result[mipp::N<float>()];
  mipp::store<float>(base, r2);
  mipp::store<float>(exponent, r2);

  for (int i = 0; i < mipp::N<float>(); i++) {
    result[i] = std::pow(base[i], exponent[i]);
  }

  mipp::reg r_pow = mipp::loadu<float>(result);
  PrintRegister<float>(r_pow, "幂结果");
}

// 类型转换示例
void MippDemo::DemoConversion() {
  std::cout << "\n===== 类型转换示例 =====\n" << std::endl;
  
  // 准备数据
  const int size = mipp::N<float>();
  float data_f[size];
  int32_t data_i[size];
  for (int i = 0; i < size; i++) {
    data_f[i] = i + 1.5f;
    data_i[i] = i + 1;
  }
  
  mipp::reg r_float = mipp::loadu<float>(data_f);
  mipp::reg r_int = mipp::loadu<int32_t>(data_i);
  
  // 1. cvt - 浮点转整数
  std::cout << "1. cvt - 浮点转整数" << std::endl;
  PrintRegister<float>(r_float, "浮点数据");
  mipp::reg r_cvt_i = mipp::cvt<float, int32_t>(r_float);
  PrintRegister<int32_t>(r_cvt_i, "转换为整数结果");
  
  // 2. cvt - 整数转浮点
  std::cout << "\n2. cvt - 整数转浮点" << std::endl;
  PrintRegister<int32_t>(r_int, "整数数据");
  mipp::reg r_cvt_f = mipp::cvt<int32_t, float>(r_int);
  PrintRegister<float>(r_cvt_f, "转换为浮点结果");
  
  // 3. round - 四舍五入
  std::cout << "\n3. round - 四舍五入" << std::endl;
  mipp::reg r_round = mipp::round<float>(r_float);
  PrintRegister<float>(r_round, "四舍五入结果");
  
  // 4. trunc - 截断
  std::cout << "\n4. trunc - 截断" << std::endl;
  mipp::reg r_trunc = mipp::trunc<float>(r_float);
  PrintRegister<float>(r_trunc, "截断结果");
}

// 对齐和内存操作示例
void MippDemo::DemoMemoryAlignment() {
  std::cout << "\n===== 对齐和内存操作示例 =====\n" << std::endl;
  
  // 1. 显示MIPP的对齐要求
  std::cout << "1. MIPP对齐要求" << std::endl;
  std::cout << "MIPP要求的内存对齐: " << mipp::RequiredAlignment << " 字节" << std::endl;
  
  // 2. 检查内存对齐
  std::cout << "\n2. 检查内存对齐" << std::endl;
  
  // 非对齐内存
  float* unaligned_ptr = new float[10];
  std::cout << "非对齐指针地址: " << unaligned_ptr << std::endl;
  std::cout << "是否对齐: " << (((uintptr_t)unaligned_ptr % mipp::RequiredAlignment) == 0 ? "是" : "否") << std::endl;
  
  // 对齐内存
  alignas(32) float aligned_data[10];
  std::cout << "对齐数组地址: " << aligned_data << std::endl;
  std::cout << "是否对齐: " << (((uintptr_t)aligned_data % mipp::RequiredAlignment) == 0 ? "是" : "否") << std::endl;
  
  // 3. 使用mipp::vector
  std::cout << "\n3. 使用mipp::vector (自动对齐)" << std::endl;
  mipp::vector<float> vec(10, 1.0f);
  std::cout << "mipp::vector地址: " << &vec[0] << std::endl;
  std::cout << "是否对齐: " << (((uintptr_t)&vec[0] % mipp::RequiredAlignment) == 0 ? "是" : "否") << std::endl;
  
  // 4. 对齐和非对齐加载性能比较
  std::cout << "\n4. 对齐和非对齐加载性能比较" << std::endl;
  const int iterations = 1000000;
  const int size = mipp::N<float>() * 10;
  
  // 准备数据
  mipp::vector<float> aligned_vec(size, 1.0f);
  float* unaligned_data = new float[size + 1];
  for (int i = 0; i < size; i++) {
    unaligned_data[i] = 1.0f;
  }
  float* unaligned_ptr_offset = unaligned_data + 1;  // 确保非对齐
  
  // 测试对齐加载性能
  auto start_aligned = std::chrono::high_resolution_clock::now();
  mipp::reg r_aligned;
  for (int i = 0; i < iterations; i++) {
    for (int j = 0; j < size; j += mipp::N<float>()) {
      r_aligned = mipp::load<float>(&aligned_vec[j]);
    }
  }
  auto end_aligned = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration_aligned = end_aligned - start_aligned;
  
  // 测试非对齐加载性能
  auto start_unaligned = std::chrono::high_resolution_clock::now();
  mipp::reg r_unaligned;
  for (int i = 0; i < iterations; i++) {
    for (int j = 0; j < size - 1; j += mipp::N<float>()) {
      r_unaligned = mipp::loadu<float>(&unaligned_ptr_offset[j]);
    }
  }
  auto end_unaligned = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> duration_unaligned = end_unaligned - start_unaligned;
  
  std::cout << "对齐加载耗时: " << duration_aligned.count() << " 毫秒" << std::endl;
  std::cout << "非对齐加载耗时: " << duration_unaligned.count() << " 毫秒" << std::endl;
  std::cout << "性能比例 (非对齐/对齐): " << duration_unaligned.count() / duration_aligned.count() << std::endl;
  
  // 清理
  delete[] unaligned_ptr;
  delete[] unaligned_data;
}

// 运行所有示例
void MippDemo::RunAllDemos() {
  std::cout << "\n========== MIPP API 示例 ==========\n" << std::endl;
  
  DemoLoadStore();
  DemoArithmetic();
  DemoComparison();
  DemoBitwise();
  DemoMasking();
  DemoShuffle();
  DemoReduction();
  DemoMathFunctions();
  DemoConversion();
  DemoMemoryAlignment();
  
  std::cout << "\n========== 示例结束 ==========\n" << std::endl;
}

}  // namespace mipp_demo
