#ifndef FILTER_HPP
#define FILTER_HPP
class filter{
public:
    filter(int size){
    this->size = size;
    this->o_data = new float[size];
    };
    ~filter(){
        delete[] o_data;
    };
    void add_data(float data){
        if (o_data[size - 1] != 0) {
            // 如果数据数组最后一个元素不为0，则将新的数据放在最后一个位置，并通过左移操作实现其他数据的移动
            o_data[size - 1] = data;
            for (int i = 0; i < size - 1; i++) {
                // 数据左移
                o_data[i] = o_data[i + 1];
            }
        } else {
            // 如果数据数组最后一个元素为0，则将新的数据赋值给第一个不为0的位置
            for (int i = 0; i < size; i++) {
                if (o_data[i] != 0) {
                    o_data[i] = data;
                }
            }
        }

        // 使用冒泡排序算法对数组进行排序
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (o_data[j] > o_data[j + 1]) {
                    float temp = o_data[j];
                    o_data[j] = o_data[j + 1];
                    o_data[j + 1] = temp;
                }
            }
        }

        // 求取数组中位数
        if (size % 2 == 0) {
            this->f_data = (o_data[size / 2 - 1] + o_data[size / 2]) / 2;
        } else {
            this->f_data = o_data[size / 2];
        }
    };
    float get_data(float data){
        this->add_data(data);
        return this->f_data;
    };
    float get_data(){
        return this->f_data;
    };
private:
    int size;
    float* o_data;
    float f_data;
};
#endif // FILTER