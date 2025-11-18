
## STM32F411 Tutorial Configuration

This tutorial provides examples for the STM32F411xE microcontroller using **VS Code** integrated with **PlatformIO**.

### Programming Style: Bare-Metal with CMSIS Headers

The examples provided rely on **direct register access** for peripheral configuration (GPIO, Timers) but utilize the standard **CMSIS headers** (`stm32f4xx.h`) for:
*   Accessing peripheral structure pointers (e.g., `RCC->`, `TIM2->`).
*   Using defined constants for register bits (e.g., `RCC_AHB1ENR_GPIOCEN`).
*   Configuring the NVIC via CMSIS functions (e.g., `NVIC_EnableIRQ`).

This approach avoids the full STM32 HAL/LL layers while leveraging CMSIS definitions for clarity and portability.

### Build Configuration

To ensure the correct initialization sequence, the startup file `startup_stm32f411xe.s` must be included in your build process.

**Procedure to include `startup_stm32f411xe.s`:**

1.  **Locate the File:** The startup file is located in the project root: `/STM32_Tutorial/startup_stm32f411xe.s`.
2.  **Copy/Link to Build Directory:** Copy or create a symbolic link to this file within your build source directory, typically `build/src/`.
    *   If using a Makefile or build script, ensure the path to this file is added to the list of assembly sources (`.S` or `.s` files) that are compiled or assembled.
    *   Example command (if using a standard build system that copies files):
        \`\`\`bash
        cp startup_stm32f411xe.s build/src/
        \`\`\`
3.  **Verification:** After adding the file, rebuild your project. The linker should now correctly use the vector table defined in this file for initialization before `main` is called.
