<project xmlns="com.autoesl.autopilot.project" name="PositionBased-ClothSim" top="process_cloth">
    <includePaths/>
    <libraryPaths/>
    <Simulation argv="">
        <SimFlow name="csim" ldflags="-L /usr/lib64" mflags="" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="../../src/testbench.cpp" sc="0" tb="1" cflags=" -Wno-unknown-pragmas" csimflags=" -Wno-unknown-pragmas" blackbox="false"/>
        <file name="src/clothkernel.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="src/clothkernel.h" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="src/process_cloth.cpp" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
        <file name="src/process_cloth.h" sc="0" tb="false" cflags="" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

