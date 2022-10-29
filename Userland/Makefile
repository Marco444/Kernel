include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: SampleCodeModule SampleDataModule 

SampleCodeModule:
	cd SampleCodeModule; make

SampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	rm -rf *.bin


.PHONY: SampleCodeModule all clean
