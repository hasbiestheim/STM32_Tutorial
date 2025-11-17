
## Build Configuration: Including Startup File

To ensure correct system initialization for these Timer examples, the startup file `startup_stm32f411xe.s` must be included in your build process, typically placed in the source directory of your build system (e.g., `build/src/`).

**Procedure:**

1.  **Locate Source:** The required startup file is located at the project root: `STM32_Tutorial/startup_stm32f411xe.s`.
2.  **Integrate into Build:** Copy or link this file into the directory where your compiler/linker expects assembly startup sources (e.g., `build/src/`).
    *   If you are using a build system that copies files from the root, ensure this file is included in the source list for assembly compilation.
    *   Example command to copy the file to a hypothetical build source directory:
        \`\`\`bash
        cp ../startup_stm32f411xe.s build/src/
        \`\`\`
3.  **Rebuild:** After integration, rebuild the project. This ensures the linker uses the correct vector table and initial stack/heap setup defined in `startup_stm32f411xe.s`.
