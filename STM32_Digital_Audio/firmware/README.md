# STM32 Digtal Audio

## I. DAC là ngoại vi gì?
- DAC (Digital-to-Analog Converter) là ngoại vi chuyển đổi tín hiệu, dùng để chuyển dữ liệu số thành tín hiệu analog.
- Chức năng chính:
    - Xuất điện áp analog từ giá trị số
    - Phát sóng sin, tam giác, âm thanh analog đơn giản
### 1. Tổng quan phần cứng DAC trên STM32F407

| Thuộc tính         | Giá trị                 |
| ------------------ | ----------------------- |
| Số kênh            | 2 (DAC1, DAC2)          |
| Độ phân giải       | 12-bit                  |
| Điện áp tham chiếu | VDDA                    |
| Giá trị xuất       | 0 → 4095                |
| Tốc độ tối đa      | ~1 MSPS (phụ thuộc tải) |

- Pinout

| Kênh DAC      | Chân MCU |
| ------------- | -------- |
| DAC Channel 1 | PA4      |
| DAC Channel 2 | PA5      |

### 2. Kiến trúc bên trong DAC
```
DMA ──► DAC_DHR ──► Holding Register ──► Output Buffer ──► PA4 / PA5
              ▲
           Trigger
```
- Các thành phần chính:
    - DHR (Data Holding Register): nơi DMA/CPU ghi dữ liệu
    - Trigger: quyết định khi nào DAC cập nhật giá trị
    - Output buffer: khuếch đại/đệm đầu ra (có thể bật/tắt)

### 3. Chế độ hoạt động chính
#### 3.1 Ghi trực tiếp bằng CPU
#### 3.2 DAC + DMA (phổ biến nhất)
```
HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1,
                  (uint32_t *)buffer,
                  size,
                  DAC_ALIGN_12B_R);
```
#### 3.3 DAC + Trigger (Timer)
- DAC chỉ cập nhật khi có trigger
- Dùng timer TRGO để điều khiển tần số
### 4. Tần số đầu ra DAC
$$
f_{out} = \frac{f_{trigger}}{N}
$$
- Trong đó:
    - $f_{trigger}$: tần số cập nhật của DAC(tần số sảy ra ngắt Update event của TIMER)
    - N: số mẫu trong buffer

- DAC hỗ trợ trigger từ:

| Trigger                            | Mô tả |
| ---------------------------------- | ----- |
| TIM2, TIM4, TIM5, TIM6, TIM7, TIM8 |       |
| EXTI Line9                         |       |
| Software trigger                   |       |
 
### 5. Điện áp đầu ra 
- đọ phân giải 

| Alignment    | Thanh ghi | Kiểu dữ liệu |
| ------------ | --------- | ------------ |
| 12-bit right | DHR12R    | uint16_t     |
| 12-bit left  | DHR12L    | uint16_t     |
| 8-bit right  | DHR8R     | uint8_t      |
-  Điện áp đầu ra 
$$
V_{out}​= \frac{D}{2^r - 1}*V_{REF+}​
$$
- Trong đó
    - D là giá trị ghi vào thanh ghi DHR
    - r là số bit độ phân giải 
    - $0 < V_{REF+} = V_{DDA} - V_{SSA} < V_{DDA}$
## 1. I2S là ngoại vi gì?

- I2S (Inter-IC Sound) là ngoại vi giao tiếp số (digital communication peripheral), chuyên dùng cho truyền dữ liệu âm thanh số.
- Thực chất I2S được triển khai dựa trên SPI (SPI + I2S mode)
- Chức năng chính:
    - Truyền/nhận dữ liệu audio số
    - Giao tiếp với codec âm thanh, DAC audio ngoài, ADC audio ngoài


