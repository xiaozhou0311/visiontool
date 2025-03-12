#ifndef ALGOTEST_H
#define ALGOTEST_H
#include "mipp.h"
#include <iostream>
#include <vector>
#include <chrono>

template<typename T>
bool isTypeAligned(T* ptr) {
    return (reinterpret_cast<uintptr_t>(ptr) % alignof(T)) == 0;
}
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

// 将MaxOp定义移到作用域外部
struct MaxOp {
    static float apply(float a, float b) { return std::max(a, b); }
};

int main() {
    // 设置控制台输出编码为UTF-8
    system("chcp 65001");

    const int Nf = mipp::N<float>();
    float real1[Nf] = { 1.0f, 2.0f, 3.0f, 4.0f };
    float imag1[Nf] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float real2[Nf] = { 2.0f, 2.0f, 2.0f, 2.0f };
    float imag2[Nf] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // 创建复数寄存器
    mipp::Regx2<float> c1(mipp::load<float>(real1), mipp::load<float>(imag1));  // 1+i, 2+i, 3+i, 4+i
    mipp::Regx2<float> c2(mipp::load<float>(real2), mipp::load<float>(imag2));  // 2+i, 2+i, 2+i, 2+i

    // 1. 复数加法 (cadd)
    mipp::Regx2<float> sum = mipp::cadd<float>(c1, c2);  // (1+2)+i(1+1), (2+2)+i(1+1), ...

    // 2. 复数减法 (csub)
    mipp::Regx2<float> diff = mipp::csub<float>(c1, c2);  // (1-2)+i(1-1), (2-2)+i(1-1), ...

    // 3. 复数乘法 (cmul)
    mipp::Regx2<float> prod = mipp::cmul<float>(c1, c2);  // (1*2-1*1)+i(1*1+1*2), ...

    // 4. 复数除法 (cdiv) - 仅支持float和double
    mipp::Regx2<float> quot = mipp::cdiv<float>(c1, c2);

    // 5. 复数乘以共轭 (cmulconj)
    mipp::Regx2<float> mulconj = mipp::cmulconj<float>(c1, c2);  // c1 * conj(c2)

    // 6. 共轭 (conj)
    mipp::Regx2<float> conj = mipp::conj<float>(c1);  // 1-i, 2-i, 3-i, 4-i

    // 7. 模的平方 (norm)
    mipp::Reg<float> norm = mipp::norm<float>(c1);  // 1²+1², 2²+1², 3²+1², 4²+1²


    // 准备测试数据
    const int N = mipp::N<float>();
    float values1[N] = { -1.0f, 2.0f, 3.0f, 4.0f };
    float values2[N] = { 2.0f, -3.0f, 4.0f, 5.0f };

    mipp::Reg<float> r1 = mipp::load<float>(values1);
    mipp::Reg<float> r2 = mipp::load<float>(values2);

    // 1. 基本算术运算
    mipp::Reg<float> sum_base = mipp::add<float>(r1, r2);    // [3,5,7,9]
    mipp::Reg<float> diff_base = mipp::sub<float>(r1, r2);   // [-1,-1,-1,-1]
    mipp::Reg<float> prod_base = mipp::mul<float>(r1, r2);   // [2,6,12,20]
    mipp::Reg<float> quot_base = mipp::div<float>(r1, r2);   // [0.5,0.67,0.75,0.8]

    // 2. 融合乘加运算（Fused Multiply-Add）
    mipp::Reg<float> r3 = mipp::set1<float>(1.0f);
    mipp::Reg<float> fma = mipp::fmadd<float>(r1, r2, r3);   // r1*r2 + r3
    mipp::Reg<float> fms = mipp::fmsub<float>(r1, r2, r3);   // r1*r2 - r3
    mipp::Reg<float> fnma = mipp::fnmadd<float>(r1, r2, r3); // -(r1*r2) + r3
    mipp::Reg<float> fnms = mipp::fnmsub<float>(r1, r2, r3); // -(r1*r2) - r3

    // 3. 最大最小值
    mipp::Reg<float> min_val = mipp::min<float>(r1, r2);  // 每个位置取较小值
    mipp::Reg<float> max_val = mipp::max<float>(r1, r2);  // 每个位置取较大值

    // 4. 快速除法
    mipp::Reg<float> div2_val = mipp::div2<float>(r1);  // 除以2
    mipp::Reg<float> div4_val = mipp::div4<float>(r1);  // 除以4

    // 5. 数学函数
    mipp::Reg<float> abs_val = mipp::abs<float>(r1);    // 绝对值
    mipp::Reg<float> sqrt_val = mipp::sqrt<float>(r1);  // 平方根
    mipp::Reg<float> rsqrt_val = mipp::rsqrt<float>(r1);// 平方根的倒数

    // 6. 饱和运算 mipp::sat<T>(r, a, b)  小于等于a的位置取a，大于b的位置取b
    mipp::Reg<float> sat_val = mipp::sat<float>(r1, 2.0f, 3.0f); // 限制在[2,3]范围

    // 7. 条件取反
    bool mask[N] = { 1,0,1,0 };
    mipp::msk m = mipp::set<N>(mask);
    mipp::Reg<float> neg_val1 = mipp::neg<float>(r1, m);     // 按掩码取反
    mipp::Reg<float> neg_val2 = mipp::neg<float>(r1, r2);    // 按r2的符号取反

    // 8. 符号判断
    mipp::Msk<mipp::N<float>()> sign_val = mipp::sign<float>(r1);

    // 9. 取整
    mipp::Reg<float> round_val = mipp::round<float>(r1);  // 四舍五入
    mipp::Reg<float> trunc_val = mipp::trunc<float>(r1);  // 截断小数部分

    // 掩码转换为寄存器值
    bool mask_trans[N] = { 1, 0, 1, 0 };
    mipp::Msk<N> m_trans = mipp::set<N>(mask_trans);
    mipp::Reg<int> r_trans = mipp::toReg<int>(m_trans);

    // float转int32_t
    float values[N] = { 1.5f, 2.7f, -3.2f, 4.8f };
    mipp::Reg<float> rf = mipp::load<float>(values);
    mipp::Reg<int32_t> ri = mipp::cvt<float, int32_t>(rf);

    // int16_t转int32_t AVX2指令集下，MIPP库没有实现从int16_t到int32_t
    /* int16_t values_int16[N] = { 1, 2, 3, 4 };
    mipp::Reg<int16_t> r16 = mipp::load<int16_t>(values_int16);
    mipp::Reg<int32_t> r32 = mipp::cvt<int16_t, int32_t>(r16);*/
    // int32_t打包为int16_t
    int32_t values3[N] = { 1, 2, 3, 4 };
    int32_t values4[N] = { 5, 6, 7, 8 };
    mipp::Reg<int32_t> r31 = mipp::load<int32_t>(values3);
    mipp::Reg<int32_t> r41 = mipp::load<int32_t>(values4);
    mipp::Reg<int16_t> packed = mipp::pack<int32_t, int16_t>(r31, r41);
    PrintRegister<int16_t>(packed, "packed");
    const int int16_t_N = mipp::N<int16_t>();

    // 准备测试数据
    float values_a[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
    float values_b[N] = { 1.0f, 1.5f, 3.0f, 5.0f };

    mipp::Reg<float> ra = mipp::load<float>(values_a);
    mipp::Reg<float> rb = mipp::load<float>(values_b);

    // 1. 等于比较 (==)
    mipp::Msk<N> eq_mask = mipp::cmpeq<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(eq_mask), "等于比较结果");  // [1,0,1,0]

    // 2. 不等于比较 (!=)
    mipp::Msk<N> neq_mask = mipp::cmpneq<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(neq_mask), "不等于比较结果");  // [0,1,0,1]

    // 3. 大于等于比较 (>=)
    mipp::Msk<N> ge_mask = mipp::cmpge<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(ge_mask), "大于等于比较结果");  // [1,1,1,0]

    // 4. 大于比较 (>)
    mipp::Msk<N> gt_mask = mipp::cmpgt<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(gt_mask), "大于比较结果");  // [0,1,0,0]

    // 5. 小于等于比较 (<=)
    mipp::Msk<N> le_mask = mipp::cmple<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(le_mask), "小于等于比较结果");  // [1,0,1,1]

    // 6. 小于比较 (<)
    mipp::Msk<N> lt_mask = mipp::cmplt<float>(ra, rb);
    PrintRegister<float>(mipp::toReg<float>(lt_mask), "小于比较结果");  // [0,0,0,1]

    // 使用比较结果进行条件操作示例
    /*
    功能说明：
    对每个位置i：
    如果掩码m[i] = 1，选择v1[i]
    如果掩码m[i] = 0，选择v2[i]
    */
    mipp::Reg<float> result = mipp::blend<float>(ra, rb, gt_mask);  // 大于时选择ra，否则选择rb
    PrintRegister<float>(result, "条件选择结果");


    // 位运算操作示例
    try {
        const int N = mipp::N<int32_t>();
        int32_t values1[N] = { 0x0F0F, 0xF0F0, 0x5555, 0xAAAA };
        int32_t values2[N] = { 0xFF00, 0x00FF, 0x3333, 0xCCCC };

        mipp::Reg<int32_t> r1 = mipp::load<int32_t>(values1);
        mipp::Reg<int32_t> r2 = mipp::load<int32_t>(values2);

        // 1. 按位与 (AND)
        mipp::Reg<int32_t> and_result = mipp::andb<int32_t>(r1, r2);
        PrintRegister<int32_t>(and_result, "按位与结果");

        // 2. 按位与非 (AND NOT)
        mipp::Reg<int32_t> andnot_result = mipp::andnb<int32_t>(r1, r2);
        PrintRegister<int32_t>(andnot_result, "按位与非结果");

        // 3. 按位或 (OR)
        mipp::Reg<int32_t> or_result = mipp::orb<int32_t>(r1, r2);
        PrintRegister<int32_t>(or_result, "按位或结果");

        // 4. 按位异或 (XOR)
        mipp::Reg<int32_t> xor_result = mipp::xorb<int32_t>(r1, r2);
        PrintRegister<int32_t>(xor_result, "按位异或结果");

        // 5. 左移 (固定位数)
        mipp::Reg<int32_t> lshift_result = mipp::lshift<int32_t>(r1, 4);
        PrintRegister<int32_t>(lshift_result, "左移4位结果");

        // 6. 左移 (可变位数)
        mipp::Reg<int32_t> shift_amount = mipp::set1<int32_t>(2);
        mipp::Reg<int32_t> lshiftr_result = mipp::lshiftr<int32_t>(r1, shift_amount);
        PrintRegister<int32_t>(lshiftr_result, "可变左移结果");

        // 7. 右移 (固定位数)
        mipp::Reg<int32_t> rshift_result = mipp::rshift<int32_t>(r1, 4);
        PrintRegister<int32_t>(rshift_result, "右移4位结果");

        // 8. 右移 (可变位数) 可变位数移位 - 每个元素可以移动不同的位数
        /*
        int32_t values[N] = {0x0F0F, 0xF0F0, 0x5555, 0xAAAA};
        mipp::Reg<int32_t> r1 = mipp::load<int32_t>(values);

        // 定义每个元素的移位数量
        int32_t shift_amounts[N] = {1, 2, 3, 4};  // 第一个元素移1位，第二个移2位...
        mipp::Reg<int32_t> amounts = mipp::load<int32_t>(shift_amounts);

        // 可变左移 - 每个元素移动不同的位数
        mipp::Reg<int32_t> lshiftr_result = mipp::lshiftr<int32_t>(r1, amounts);
        // 输入：  [0x0F0F,     0xF0F0,     0x5555,     0xAAAA]
        // 移位：  [1位,        2位,        3位,        4位]
        // 输出：  [0x1E1E,    0x3C3C0,    0x2AAA8,    0xAAA0]
        */
        mipp::Reg<int32_t> rshiftr_result = mipp::rshiftr<int32_t>(r1, shift_amount);
        PrintRegister<int32_t>(rshiftr_result, "可变右移结果");

        // 9. 按位取反 (NOT)
        mipp::Reg<int32_t> not_result = mipp::notb<int32_t>(r1);
        PrintRegister<int32_t>(not_result, "按位取反结果");

        // 掩码位运算示例
        bool mask1[N] = { 1, 0, 1, 0 };
        bool mask2[N] = { 0, 1, 1, 0 };
        mipp::Msk<N> m1 = mipp::set<N>(mask1);
        mipp::Msk<N> m2 = mipp::set<N>(mask2);

        // 10. 掩码按位与
        mipp::Msk<N> mask_and = mipp::andb<N>(m1, m2);
        PrintRegister<int32_t>(mipp::toReg<int32_t>(mask_and), "掩码按位与结果");

        // 11. 掩码按位或
        mipp::Msk<N> mask_or = mipp::orb<N>(m1, m2);
        PrintRegister<int32_t>(mipp::toReg<int32_t>(mask_or), "掩码按位或结果");

        // 12. 掩码按位异或
        mipp::Msk<N> mask_xor = mipp::xorb<N>(m1, m2);
        PrintRegister<int32_t>(mipp::toReg<int32_t>(mask_xor), "掩码按位异或结果");

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    {
        const int N = mipp::N<float>();

        // 1. 基本数据准备
        float values[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float output[N];
        float unaligned_values[N] = { 5.0f, 6.0f, 7.0f, 8.0f };

        // 2. 对齐和非对齐加载
        mipp::Reg<float> r1 = mipp::load<float>(values);        // 对齐加载
        mipp::Reg<float> r2 = mipp::loadu<float>(unaligned_values);  // 非对齐加载
        PrintRegister<float>(r1, "对齐加载结果");
        PrintRegister<float>(r2, "非对齐加载结果");

        // 3. 对齐和非对齐存储
        mipp::Reg<float> store_val = mipp::set1<float>(1.0f);   // 所有元素设为1.0
        mipp::store<float>(output, store_val);                   // 对齐存储
        mipp::storeu<float>(output, store_val);                  // 非对齐存储

        // 4. 掩码操作
        /*
        // 创建掩码 [1, 0, 1, 0]
        bool mask[N] = {1, 0, 1, 0};
        mipp::Msk<N> m = mipp::set<N>(mask);

        // 假设 values = [1.0, 2.0, 3.0, 4.0]
        mipp::Reg<float> masked_load = mipp::maskzld<float>(m, values);
        // 结果: [1.0, 0.0, 3.0, 0.0]
        // 解释: 掩码为1的位置保留原值，为0的位置设为0

        // 假设 r1 = [5.0, 6.0, 7.0, 8.0]
        mipp::maskst<float>(m, output, r1);
        // output数组结果: [5.0, 保持不变, 7.0, 保持不变]
        // 解释: 掩码为1的位置更新为r1的值，为0的位置保持output原值
        */
        bool mask[N] = { 1, 0, 1, 0 };
        mipp::Msk<N> m = mipp::set<N>(mask);
        mipp::Reg<float> masked_load = mipp::maskzld<float>(m, values);  // 掩码加载
        mipp::maskst<float>(m, output, r1);                              // 掩码存储
        PrintRegister<float>(masked_load, "掩码加载结果");

        // 5. 特殊加载函数
        mipp::Reg<float> broadcast = mipp::set1<float>(3.14f);   // 广播单值
        mipp::Reg<float> zeros = mipp::set0<float>();           // 设置为0
        float init_vals[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        mipp::Reg<float> from_array = mipp::set<float>(init_vals); // 从数组设置
        PrintRegister<float>(broadcast, "广播结果");
        PrintRegister<float>(zeros, "零寄存器");
        PrintRegister<float>(from_array, "从数组设置结果");

        // 6. 数据访问
        float val = mipp::get<float>(r1, 2);          // 获取特定位置的值
        float first = mipp::getfirst<float>(r1);      // 获取第一个元素
        std::cout << "第3个元素: " << val << std::endl;
        std::cout << "第1个元素: " << first << std::endl;

        // 7. 高低部分操作
        mipp::Reg_2<float> low_part = mipp::low<float>(r1);   // 获取低半部分
        mipp::Reg_2<float> high_part = mipp::high<float>(r1); // 获取高半部分
        mipp::Reg<float> combined = mipp::combine<float>(low_part, high_part); // 合并
        PrintRegister<float>(combined, "合并结果");
    }

    {
        const int N = mipp::N<float>();

        // 1. 准备测试数据
        float values[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        mipp::Reg<float> r = mipp::load<float>(values);
        PrintRegister<float>(r, "原始数据");

        // 2. 归约操作
        // 2.1 求和归约
        float sum = mipp::hadd<float>(r);
        std::cout << "向量元素和 (hadd): " << sum << std::endl;

        // 2.2 乘积归约
        float product = mipp::hmul<float>(r);
        std::cout << "向量元素乘积 (hmul): " << product << std::endl;

        // 2.3 最小值归约
        float min_val = mipp::hmin<float>(r);
        std::cout << "向量最小值 (hmin): " << min_val << std::endl;

        // 2.4 最大值归约
        float max_val = mipp::hmax<float>(r);
        std::cout << "向量最大值 (hmax): " << max_val << std::endl;

        // 3. 零测试
        // 3.1 单个寄存器零测试
        mipp::Reg<float> zeros = mipp::set0<float>();
        bool is_zero = mipp::testz<float>(zeros);
        std::cout << "零寄存器测试 (testz): " << (is_zero ? "全为0" : "不全为0") << std::endl;

        // 3.2 两个寄存器按位与后零测试
        mipp::Reg<float> r2 = mipp::set1<float>(1.0f);
        bool is_and_zero = mipp::testz<float>(r, r2);
        std::cout << "寄存器按位与零测试 (testz): " << (is_and_zero ? "与结果全为0" : "与结果不全为0") << std::endl;

        // 3.3 掩码零测试
        bool mask[N] = { 0, 0, 0, 0 };
        mipp::Msk<N> m = mipp::set<N>(mask);
        bool is_mask_zero = mipp::testz<N>(m);
        std::cout << "掩码零测试 (testz): " << (is_mask_zero ? "掩码全为0" : "掩码不全为0") << std::endl;

        // 4. 数学函数
        // 4.1 准备角度数据
        float angle_values[N] = { 0.0f, M_PI / 6, M_PI / 4, M_PI / 3 };
        mipp::Reg<float> angles = mipp::load<float>(angle_values);
        PrintRegister<float>(angles, "角度值（弧度）");

        // 4.2 三角函数
        mipp::Reg<float> sin_vals = mipp::sin<float>(angles);
        mipp::Reg<float> cos_vals = mipp::cos<float>(angles);
        mipp::Reg<float> tan_vals = mipp::tan<float>(angles);
        PrintRegister<float>(sin_vals, "正弦值");
        PrintRegister<float>(cos_vals, "余弦值");
        PrintRegister<float>(tan_vals, "正切值");

        // 4.3 同时计算正弦和余弦
        mipp::Regx2<float> sincos_vals = mipp::sincos<float>(angles);
        mipp::Reg<float> sin_part = sincos_vals.val[0];
        mipp::Reg<float> cos_part = sincos_vals.val[1];
        PrintRegister<float>(sin_part, "sincos正弦部分");
        PrintRegister<float>(cos_part, "sincos余弦部分");

        // 4.4 指数和对数
        mipp::Reg<float> exp_vals = mipp::exp<float>(r);
        mipp::Reg<float> log_vals = mipp::log<float>(r);
        PrintRegister<float>(exp_vals, "指数值");
        PrintRegister<float>(log_vals, "对数值");

        // 4.5 双曲函数
        mipp::Reg<float> sinh_vals = mipp::sinh<float>(angles);
        mipp::Reg<float> cosh_vals = mipp::cosh<float>(angles);
        mipp::Reg<float> tanh_vals = mipp::tanh<float>(angles);
        PrintRegister<float>(sinh_vals, "双曲正弦值");
        PrintRegister<float>(cosh_vals, "双曲余弦值");
        PrintRegister<float>(tanh_vals, "双曲正切值");

        // 4.6 反双曲函数
        mipp::Reg<float> asinh_vals = mipp::asinh<float>(r);
        mipp::Reg<float> acosh_vals = mipp::acosh<float>(exp_vals); // 使用exp_vals因为acosh需要>=1的输入
        mipp::Reg<float> atanh_vals = mipp::atanh<float>(tanh_vals);
        PrintRegister<float>(asinh_vals, "反双曲正弦值");
        PrintRegister<float>(acosh_vals, "反双曲余弦值");
        PrintRegister<float>(atanh_vals, "反双曲正切值");

        // 5. 自定义归约操作示例（如果需要）
        // 使用Reduction类进行自定义归约操作
        float custom_max = mipp::Reduction<float, MaxOp>::sapply(r);
        std::cout << "自定义最大值归约: " << custom_max << std::endl;
    }

    {
        const int N = mipp::N<float>();

        // 1. 基础数据准备
        float values1[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float values2[N] = { 5.0f, 6.0f, 7.0f, 8.0f };
        mipp::Reg<float> r1 = mipp::load<float>(values1);
        mipp::Reg<float> r2 = mipp::load<float>(values2);

        // 1. compress操作 - 根据掩码压缩数据
        bool mask_vals[N] = { 1, 0, 1, 0 };
        mipp::Msk<N> mask = mipp::set<N>(mask_vals);
        mipp::Reg<float> compressed = mipp::compress<float>(r1, mask);
        PrintRegister<float>(compressed, "压缩结果");  // [1.0, 3.0, x, x]

        // 2. cmask系列操作 - 创建重排掩码
        uint32_t ids[N] = { 3, 2, 1, 0 };           // 完全重排
        uint32_t ids2[N / 2] = { 1, 0 };              // 两路重排 两路重排：每两个元素交换
        uint32_t ids4[N / 4] = { 0 };                 // 四路重排 四路重排：每四个元素交换

        mipp::Reg<float> cm = mipp::cmask<float>(ids);      // 完全重排掩码
        mipp::Reg<float> cm2 = mipp::cmask2<float>(ids2);   // 两路重排掩码
        mipp::Reg<float> cm4 = mipp::cmask4<float>(ids4);   // 四路重排掩码

        // 3. shuff系列操作 - 使用掩码重排数据
        mipp::Reg<float> shuffled = mipp::shuff<float>(r1, cm);    // 完全重排
        mipp::Reg<float> shuffled2 = mipp::shuff2<float>(r1, cm2); // 两路重排
        mipp::Reg<float> shuffled4 = mipp::shuff4<float>(r1, cm4); // 四路重排

        PrintRegister<float>(shuffled, "完全重排结果");     // [4.0, 3.0, 2.0, 1.0]
        PrintRegister<float>(shuffled2, "两路重排结果");    // [2.0, 1.0, 4.0, 3.0]
        PrintRegister<float>(shuffled4, "四路重排结果");    // [1.0, 2.0, 3.0, 4.0]

        // 4. interleave系列操作 - 交织数据
        // 4.1 基本交织
        mipp::Regx2<float> interleaved = mipp::interleave<float>(r1, r2);
        PrintRegister<float>(interleaved.val[0], "完全交织低部分");  // [1.0, 5.0, 2.0, 6.0]
        PrintRegister<float>(interleaved.val[1], "完全交织高部分");  // [3.0, 7.0, 4.0, 8.0]

        // 4.2 解交织
        mipp::Regx2<float> deinterleaved = mipp::deinterleave<float>(interleaved.val[0], interleaved.val[1]);
        PrintRegister<float>(deinterleaved.val[0], "解交织结果1");
        PrintRegister<float>(deinterleaved.val[1], "解交织结果2");

        // 4.3 分段交织
        mipp::Regx2<float> interleaved2 = mipp::interleave2<float>(r1, r2);
        mipp::Regx2<float> interleaved4 = mipp::interleave4<float>(r1, r2);

        // 4.4 低位交织
        mipp::Reg<float> interleavelo_result = mipp::interleavelo<float>(r1, r2);
        mipp::Reg<float> interleavelo2_result = mipp::interleavelo2<float>(r1, r2);
        mipp::Reg<float> interleavelo4_result = mipp::interleavelo4<float>(r1, r2);

        PrintRegister<float>(interleavelo_result, "低位交织结果");
        PrintRegister<float>(interleavelo2_result, "两路低位交织结果");
        PrintRegister<float>(interleavelo4_result, "四路低位交织结果");

        // 4.5 高位交织
        mipp::Reg<float> interleavehi_result = mipp::interleavehi<float>(r1, r2);
        mipp::Reg<float> interleavehi2_result = mipp::interleavehi2<float>(r1, r2);
        mipp::Reg<float> interleavehi4_result = mipp::interleavehi4<float>(r1, r2);

        PrintRegister<float>(interleavehi_result, "高位交织结果");
        PrintRegister<float>(interleavehi2_result, "两路高位交织结果");
        PrintRegister<float>(interleavehi4_result, "四路高位交织结果");

        // 5. 旋转操作
        mipp::Reg<float> left_rotated = mipp::lrot<float>(r1);   // 左旋转
        mipp::Reg<float> right_rotated = mipp::rrot<float>(r1);  // 右旋转

        PrintRegister<float>(left_rotated, "左旋转结果");   // [2.0, 3.0, 4.0, 1.0]
        PrintRegister<float>(right_rotated, "右旋转结果");  // [4.0, 1.0, 2.0, 3.0]

        // 6. 混合操作
        bool blend_mask[N] = { 1, 0, 1, 0 };
        mipp::Msk<N> m = mipp::set<N>(blend_mask);

        // blend和select功能相同，只是参数顺序不同
        mipp::Reg<float> blended = mipp::blend<float>(r1, r2, m);
        mipp::Reg<float> selected = mipp::select<float>(m, r1, r2);

        PrintRegister<float>(blended, "blend结果");   // [1.0, 6.0, 3.0, 8.0]
        PrintRegister<float>(selected, "select结果"); // [1.0, 6.0, 3.0, 8.0]
    }

    {
        const int N = mipp::N<float>();

        // 准备数据
        float values1[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float values2[N] = { 5.0f, 6.0f, 7.0f, 8.0f };
        mipp::Reg<float> r1 = mipp::load<float>(values1);
        mipp::Reg<float> r2 = mipp::load<float>(values2);

        // 1. 基本交织操作 interleave
        mipp::Regx2<float> interleaved = mipp::interleave<float>(r1, r2);
        PrintRegister<float>(interleaved.val[0], "基本交织低部分");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8]
        // 输出: val[0]=[1,5,2,6], val[1]=[3,7,4,8]
        PrintRegister<float>(interleaved.val[1], "基本交织高部分");

        // 2. 解交织操作 deinterleave
        mipp::Regx2<float> deinterleaved = mipp::deinterleave<float>(interleaved.val[0], interleaved.val[1]);
        PrintRegister<float>(deinterleaved.val[0], "解交织结果1"); // [1,2,3,4]
        PrintRegister<float>(deinterleaved.val[1], "解交织结果2"); // [5,6,7,8]

        // 3. 两路交织 interleave2
        mipp::Regx2<float> interleaved2 = mipp::interleave2<float>(r1, r2);
        PrintRegister<float>(interleaved2.val[0], "两路交织低部分");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8]
        // 输出: 在每两个元素内进行交织
        // val[0]=[1,5,2,6], val[1]=[3,7,4,8]

        // 4. 四路交织 interleave4
        mipp::Regx2<float> interleaved4 = mipp::interleave4<float>(r1, r2);
        PrintRegister<float>(interleaved4.val[0], "四路交织低部分");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8]
        // 输出: 在每四个元素内进行交织

        // 5. 低位交织操作
        // 5.1 基本低位交织
        mipp::Reg<float> interleavelo = mipp::interleavelo<float>(r1, r2);
        PrintRegister<float>(interleavelo, "基本低位交织");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8]
        // 输出: [1,5,2,6]

        // 5.2 两路低位交织
        mipp::Reg<float> interleavelo2 = mipp::interleavelo2<float>(r1, r2);
        PrintRegister<float>(interleavelo2, "两路低位交织");
        // 在每两个元素内进行低位交织

        // 5.3 四路低位交织
        mipp::Reg<float> interleavelo4 = mipp::interleavelo4<float>(r1, r2);
        PrintRegister<float>(interleavelo4, "四路低位交织");
        // 在每四个元素内进行低位交织

        // 6. 高位交织操作
        // 6.1 基本高位交织
        mipp::Reg<float> interleavehi = mipp::interleavehi<float>(r1, r2);
        PrintRegister<float>(interleavehi, "基本高位交织");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8]
        // 输出: [3,7,4,8]

        // 6.2 两路高位交织
        mipp::Reg<float> interleavehi2 = mipp::interleavehi2<float>(r1, r2);
        PrintRegister<float>(interleavehi2, "两路高位交织");
        // 在每两个元素内进行高位交织

        // 6.3 四路高位交织
        mipp::Reg<float> interleavehi4 = mipp::interleavehi4<float>(r1, r2);
        PrintRegister<float>(interleavehi4, "四路高位交织");
        // 在每四个元素内进行高位交织
    }

    {
        const int N = mipp::N<float>();

        // 准备数据
        float values[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        mipp::Reg<float> r = mipp::load<float>(values);

        // 1. 左旋转 (lrot)
        mipp::Reg<float> left_rotated = mipp::lrot<float>(r);
        PrintRegister<float>(left_rotated, "左旋转结果");
        // 输入: [1.0, 2.0, 3.0, 4.0]
        // 输出: [2.0, 3.0, 4.0, 1.0]
        // 解释: 所有元素向左移动一位，第一个元素移到最后

        // 2. 右旋转 (rrot)
        mipp::Reg<float> right_rotated = mipp::rrot<float>(r);
        PrintRegister<float>(right_rotated, "右旋转结果");
        // 输入: [1.0, 2.0, 3.0, 4.0]
        // 输出: [4.0, 1.0, 2.0, 3.0]
        // 解释: 所有元素向右移动一位，最后一个元素移到最前
    }

    {
        const int N = mipp::N<float>();

        // 准备数据
        float values1[N] = { 1.0f, 2.0f, 3.0f, 4.0f };
        float values2[N] = { 5.0f, 6.0f, 7.0f, 8.0f };
        mipp::Reg<float> r1 = mipp::load<float>(values1);
        mipp::Reg<float> r2 = mipp::load<float>(values2);

        // 创建掩码 [1, 0, 1, 0]
        bool mask_vals[N] = { 1, 0, 1, 0 };
        mipp::Msk<N> mask = mipp::set<N>(mask_vals);

        // 1. blend操作
        mipp::Reg<float> blended = mipp::blend<float>(r1, r2, mask);
        PrintRegister<float>(blended, "blend结果");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8], mask=[1,0,1,0]
        // 输出: [1,6,3,8]
        // 解释: mask=1时选择r1的值，mask=0时选择r2的值

        // 2. select操作
        mipp::Reg<float> selected = mipp::select<float>(mask, r1, r2);
        PrintRegister<float>(selected, "select结果");
        // 输入: r1=[1,2,3,4], r2=[5,6,7,8], mask=[1,0,1,0]
        // 输出: [1,6,3,8]
        // 解释: mask=1时选择r1的值，mask=0时选择r2的值
    }
    return 0;
}

#endif