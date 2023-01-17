# Partial Reconfiguration 
A dynamic partial reconfiguraton tutorial for ZYBO and MPSoC ZU106 using PCAP.

Part of my thesis research

<hr>
In this tutorial, I developed a simple module that performs addition and subtraction on two memory maped axi-lite signal and return the result in the same interface. 

<hr>
The steps may not be optimal and some elements could be excluded. 

## Vivado 
- Create a block design
- Add the Axi-Lite communication module | Status
  - The decupler and the gpio are not used 
- Make external the ports of the AXI 
- Create a top module that include the block design and the reconfigurable part. 
- Open Syntesised Design and floorplan the reconfigurable design and area. Save it the .xdc file. 
- In the vivado tools click on **Enable Dynamic Function eXchange**
- Hover above the sources, select the reconfigurable module, right click on **Create Partition Definition** with the name you like
- In the vivado tools click on **Dynamic Function eXchange Wizard**
  - Add your module to the partition | model it by view the existing one in the edit. (For logic design, only the file is needed).
  - Click on next, to the **Edit Configurations** tab. 
  - Add the different configurations and select the different logic to each of the configurations. 
  - Click on next, to the **Edit Configurations Runs** tab. 
  - Select the automatic configuration. 
  - Finish from the Wizard

- Generate the Bitstream. 
- In the design runs, there is a report, and on the vivado runs folder, there are the full bitstream and the partial ones, labeled impl1 and the name gives respectively. 

- Export the hardware and get to vitis. 

## Vitis
- In the .spr and the cpu tab modify the bsp settings to add the nessesary libraries to the Board Support Package. 
  - Zybo : xilffs
  - MPSoC : xilffs,xilfpga, xilsecure
- In the application lscript.ld adjust the heap size to be able to store the partial bitstream. 


## Block Design
### MPSoC block design
![](/images/mpsoc_block_desing.png)

## Credits 
- [Youtube video](https://www.youtube.com/watch?v=bY-ik39QI8w&list=PL9GWVTghqmkJKUHYGx-59WihvVO-d0y-f&index=2&ab_channel=MohammadS.Sadri) that explains someof this stuff. He is a bad person, putting the code behind a pay wall but at least he helped understand some of the basics. 
- [Youtube video](https://www.youtube.com/watch?v=kT74m-H31a8&t=1177s&ab_channel=VipinKizheppatt) and [GitHub](https://github.com/vipinkmenon/PCAP). He has different repos but that helped me the most. 