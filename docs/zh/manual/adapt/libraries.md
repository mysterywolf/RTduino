# 如何将Arduino库适配RTduino

Arduino部分库会根据不同的架构（包括CPU架构或不同板子的结构），进行不同的适配，对于RTduino，识别宏为 `ARDUINO_ARCH_RTTHREAD`。请参考此 [commit](https://github.com/PaulStoffregen/CapacitiveSensor/commit/25dd066f412af0c988aa3712bebfcb263c9054e0#diff-5957e867d92ebf881ddfc665f29824357eab87f987c6097dc8958d9053c6e6f7R387) 进行适配