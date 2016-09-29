# TextExtractor-UE4
============

Unreal Engine 4 integraton of [Apache Tika™](http://tika.apache.org/), a java based library to detect and extract metadata and text from over a thousand different file types (such as PPT, XLS, and PDF).

For more details on the formats supported: [Supported Formats](https://tika.apache.org/1.4/formats.html)

## Srategy 

A first C++ wrapper is taking care of the creation of a Java Virtual Machine(JVM) as well as loading the Tika Java library via JNI.

It is then encapsulated in a DLL which is dynamically loaded by UE4 and called from a blueprint library.

## Warning
Since the plugin is using a Java virtual machine, you will see memory exceptions if you are using Visual Studio. 
You can just skip them. 
