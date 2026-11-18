# Real-Time VGA Test Pattern Generator — Zybo Z7-20

EEL4740 Embedded Systems project, Florida International University.

The design outputs video test patterns (color bars, gradients, checkerboards, and others) over VGA from a Digilent **Zybo Z7-20** (Xilinx Zynq-7000). Patterns like these are used to test and calibrate TVs and monitors and to find dead pixels. The board's switches select which pattern is shown.

**Team:** Gregory Bastidas, Eduardo Garcia, Andres Linares, Sebastian Rodriguez

The full write-up, with the block design diagram, photos of the output, code, and troubleshooting notes, is in [docs/VGA_Test_Pattern_Generator_Report.pdf](docs/VGA_Test_Pattern_Generator_Report.pdf).

## Hardware

- Zybo Z7-20 development board
- VGA Pmod on headers **JC** (red/blue) and **JD** (green/HSync/VSync)
- VGA cable and a VGA-capable monitor
- Micro-USB cable for programming and UART

## Design

The design is a Vivado block design (`VGA_BD`) built from these IP cores:

- Zynq7 Processing System
- Video Test Pattern Generator (TPG)
- Video Timing Controller (VTC)
- AXI4-Stream to Video Out
- Clocking Wizard
- Processor System Reset
- AXI GPIO (switch input)

The Zynq PS runs software written in Xilinx SDK / Vitis. It initializes the TPG and VTC and switches patterns based on the switch inputs.

## Repository layout

```
VGA_TPG/
  VGA_TPG.xpr                   Vivado project
  VGA_TPG.srcs/                 Block design, IP configuration, constraints
  VGA_BD_wrapper.bit            Prebuilt bitstream
  VGA_BD_wrapper.xsa            Hardware export for Vitis
  VGA_BD_Patterns_wrapper.xsa   Hardware export (multi-pattern version)
  ps7_init.*                    Zynq PS initialization files
constraints/
  Zybo-Z7-Master.xdc            Pin constraints (reset button, VGA Pmod JC/JD)
docs/
  VGA_Test_Pattern_Generator_Report.pdf
```

Vivado's generated output folders (`.cache`, `.runs`, `.ip_user_files`, `.sim`) are not committed. Vivado recreates them when you open the project and run synthesis.

## Building

1. Open `VGA_TPG/VGA_TPG.xpr` in Vivado (the project was built with 2020.2).
2. Run **Generate Bitstream**, or program the board with the included `VGA_BD_wrapper.bit`.
3. In Vitis, create a platform from `VGA_BD_wrapper.xsa`, add the application code, and run it on the board.

### Known issue: Vivado VTC / HLS bug (y2k22)

Vivado 2014 through 2021 fails to synthesize and implement HLS-based video IP, including the Video Timing Controller and TPG used here, because of a date-overflow bug in IP export. Install Xilinx's **y2k22 patch** (AMD/Xilinx Answer Record 76960) before you build. See the Troubleshooting section of the report for details.

## Future work

- Add button controls for real-time interaction
- Move to HDMI for higher resolution and modern displays
- Add animations and real-time graphics
