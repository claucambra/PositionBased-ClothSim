<project xmlns="com.autoesl.autopilot.project" name="ClothSim" top="process_cloth">
    <includePaths/>
    <libraryPaths/>
    <Simulation>
        <SimFlow name="csim" profile="true" clean="true" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="../../main.cpp" sc="0" tb="1" cflags=" -Wno-unknown-pragmas" csimflags=" -Wno-unknown-pragmas" blackbox="false"/>
        <file name="Vitis_HLS/src/process_cloth.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="Vitis_HLS/src/process_cloth.h" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="Vitis_HLS/src/clothkernel.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="Vitis_HLS/src/clothkernel.h" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

