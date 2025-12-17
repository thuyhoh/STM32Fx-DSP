# STM32 DSP
## I. Thêm tư viện CMSIS-DSP

### 1. Cấu trúc một project

- Copy folders in the created folder
    - <STM32Cube_Repository>\STM32Cube_FW_F4_V1.28.0\Drivers\CMSIS\DSP\Include
    - <STM32Cube_Repository>\STM32Cube_FW_F4_V1.28.0\Drivers\CMSIS\DSP\PrivateInclude
    - <STM32Cube_Repository>\STM32Cube_FW_F4_V1.28.0\Drivers\CMSIS\DSP\Source

![image](./Images/P1.png)

### 2. Add include path
![image](./Images/P2.png)

### 3. Chỉnh sửa file 
- Open SupportFunctions.c and SupportFunctionsF16.c files and comment these lines of code:
    - SupportFunctions.c 
    ``` C
    #include "arm_barycenter_f32.c"
    #include "arm_bitonic_sort_f32.c"
    #include "arm_bubble_sort_f32.c"
    #include "arm_copy_f32.c"
    #include "arm_copy_f64.c"
    #include "arm_copy_q15.c"
    #include "arm_copy_q31.c"
    #include "arm_copy_q7.c"
    #include "arm_fill_f32.c"
    #include "arm_fill_f64.c"
    #include "arm_fill_q15.c"
    #include "arm_fill_q31.c"
    #include "arm_fill_q7.c"
    #include "arm_heap_sort_f32.c"
    #include "arm_insertion_sort_f32.c"
    #include "arm_merge_sort_f32.c"
    #include "arm_merge_sort_init_f32.c"
    #include "arm_quick_sort_f32.c"
    #include "arm_selection_sort_f32.c"
    #include "arm_sort_f32.c"
    #include "arm_sort_init_f32.c"
    #include "arm_weighted_sum_f32.c"

    #include "arm_float_to_q15.c"
    #include "arm_float_to_q31.c"
    #include "arm_float_to_q7.c"
    #include "arm_q15_to_float.c"
    #include "arm_q15_to_q31.c"
    #include "arm_q15_to_q7.c"
    #include "arm_q31_to_float.c"
    #include "arm_q31_to_q15.c"
    #include "arm_q31_to_q7.c"
    #include "arm_q7_to_float.c"
    #include "arm_q7_to_q15.c"
    #include "arm_q7_to_q31.c"
    ```
    - SupportFunctionsF16.c
    ``` C
    #include "arm_copy_f16.c"
    #include "arm_fill_f16.c"
    #include "arm_f16_to_q15.c"
    #include "arm_f16_to_float.c"
    #include "arm_q15_to_f16.c"
    #include "arm_float_to_f16.c"
    #include "arm_weighted_sum_f16.c"
    #include "arm_barycenter_f16.c"
    ```