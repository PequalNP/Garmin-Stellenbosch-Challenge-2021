#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wav.h"

// heavily inspired by code from http://truelogic.org/wordpress/2015/09/04/parsing-a-wav-file-in-c/
// author: https://github.com/PequalNP

using namespace std;
 
 

FILE* audiofile;


struct HEADER header;
int read;

unsigned char buffer4[4];
unsigned char buffer2[2];

int main(){
    const char *fileNames[3] = {"Transmit_1","Shotput_receive_1","Javelin_receive_1"};
    for (int i = 0; i<3; i++){
        int mSize = sizeof(fileNames[i])/sizeof(fileNames[i][0]);
   
        char inputFilename [mSize+4];
        strcpy(inputFilename,fileNames[i]); 
        strcat(inputFilename,".wav");
        audiofile = fopen(inputFilename,"rb");
        if (audiofile==NULL) {
            cout << "file not opened"<< endl; 
            exit (1);
            }

        read = fread(header.riff, sizeof(header.riff),1,audiofile);
        cout <<"Chunk ID: "<< header.riff << endl;
        read = fread(buffer4,sizeof(buffer4),1,audiofile);

        header.overall_size = (buffer4[3]<<24)|
                            (buffer4[2]<<16)|
                            (buffer4[1]<<8)|
                            buffer4[0];
        cout <<"Chunk Size:" <<header.overall_size << endl;

        read = fread(header.wave, sizeof(header.wave), 1, audiofile); // format
        cout << "format: "<< header.wave << endl;

        read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, audiofile); //subchunk1 ID
        cout << "1st subchunk ID: "<< header.fmt_chunk_marker << endl;

        read = fread(buffer4, sizeof(buffer4), 1, audiofile);

        // convert little endian to big endian 4 byte integer
        header.length_of_fmt = (buffer4[3]<<24)|
                            (buffer4[2]<<16)|
                            (buffer4[1]<<8)|
                            buffer4[0];
        cout << "1st Subchunk size: " << header.length_of_fmt << endl;
        
        read = fread(buffer2, sizeof(buffer2), 1, audiofile);

        header.format_type = (buffer2[1] << 8)|
                            buffer2[0];

        string format_name;

        if (header.format_type == 1){
            format_name= "PCM";
        }else if(header.format_type == 6){
            format_name= "A-law";
        }else if (header.format_type == 7){
            format_name= "Mu-law";
        }

        cout << "Format type: " << format_name << endl;

        read = fread(buffer2, sizeof(buffer2), 1, audiofile); // channels
        header.channels = (buffer2[1] << 8)| buffer2[0] ; // channels
        cout << "Channels: " << header.channels << endl;
        
        read = fread(buffer4, sizeof(buffer4), 1, audiofile);
        header.sample_rate = (buffer4[3]<<24)|
                            (buffer4[2]<<16)|
                            (buffer4[1]<<8)|
                            buffer4[0];

        cout<< "Sample rate: " << header.sample_rate << endl;

        read = fread(buffer4, sizeof(buffer4), 1, audiofile);
        header.byterate = (buffer4[3]<<24)|
                            (buffer4[2]<<16)|
                            (buffer4[1]<<8)|
                            buffer4[0];
        
        cout<< "Byte rate: " << header.byterate << endl;

        read = fread(buffer2, sizeof(buffer2), 1, audiofile);
        header.block_align = (buffer2[1]<<8)|
                        buffer2[0];
        
        cout << "Block align: " <<header.block_align << endl;

        read = fread(buffer2, sizeof(buffer2), 1, audiofile);
        header.bits_per_sample = (buffer2[1]<<8)|
                        buffer2[0];
        
        cout << "Bits per sample: " << header.bits_per_sample << endl;


        read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, audiofile);
        cout << "2nd subchunk ID: "<< header.data_chunk_header << endl;

        read = fread(buffer4, sizeof(buffer4), 1, audiofile);
        header.data_size = (buffer4[3]<<24)|
                        (buffer4[2]<<16)|
                        (buffer4[1]<<8)|
                        buffer4[0];
        cout << "2nd subchunk size: "<< header.data_size << endl;
        // calculate no.of samples
        long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);

        // size of each sample
        long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;

        // calculate duration of file
        float duration_in_seconds = (float) header.overall_size / header.byterate;
        cout<<"Approx.Duration in seconds="<<duration_in_seconds<<endl;

        // obtain file size:
       

        // // allocate memory to contain the whole file:
        // char* bufferData = (char*) malloc (sizeof(char)*lSize);
        // if (bufferData == NULL) {fputs ("Memory error",stderr); exit (2);}

        // // copy the file into the buffer:
        // read = fread (bufferData,1,lSize,transmit);
        // if (read != lSize) {fputs ("Reading error",stderr); exit (3);}

        // data_file = fopen("data.bin","wb");
        char outputFilename [mSize+4];
        strcpy(outputFilename,fileNames[i]); 
        strcat(outputFilename,".txt");
        ofstream data_file(outputFilename);
        // fwrite(bufferData,sizeof(char),sizeof(bufferData),data_file);

        // fclose(transmit);
        // fclose(data_file);

        if (header.format_type == 1){
            char data_buffer[size_of_each_sample];
            bool size_is_correct = true;

            long bytes_in_each_channel = (size_of_each_sample/header.channels);
            if ((bytes_in_each_channel * header.channels) != size_of_each_sample){
                printf("Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
                size_is_correct = false;
            }

            if (size_is_correct) { 
                        // the valid amplitude range for values based on the bits per sample
                long low_limit = 0l;
                long high_limit = 0l;

                switch (header.bits_per_sample) {
                    case 8:
                        low_limit = -128;
                        high_limit = 127;
                        break;
                    case 16:
                        low_limit = -32768;
                        high_limit = 32767;
                        break;
                    case 32:
                        low_limit = -2147483648;
                        high_limit = 2147483647;
                        break;
                }
        
            
                for (long i = 0; i <= num_samples; i++){
                    read = fread(data_buffer, sizeof(data_buffer),1 , audiofile);
                    if (read == 1){
                    // dump the data read
                        unsigned int  xchannels = 0;
                        int data_in_channel = 0;
                        int offset = 0; // move the offset for every iteration in the loop below
                        for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
                            // convert data from little endian to big endian based on bytes in each channel sample
                            if (bytes_in_each_channel == 4) {
                                data_in_channel = (data_buffer[offset] & 0x00ff) | 
                                                    ((data_buffer[offset + 1] & 0x00ff) <<8) | 
                                                    ((data_buffer[offset + 2] & 0x00ff) <<16) | 
                                                    (data_buffer[offset + 3]<<24);
                            }
                            else if (bytes_in_each_channel == 2) {
                                data_in_channel = (data_buffer[offset] & 0x00ff) |
                                                    (data_buffer[offset + 1] << 8);
                            }
                            else if (bytes_in_each_channel == 1) {
                                data_in_channel = data_buffer[offset] & 0x00ff;
                                data_in_channel -= 128; //in wave, 8-bit are unsigned, so shifting to signed
                            }

                            offset += bytes_in_each_channel;	
                            data_file << 	data_in_channel << endl;

                            // check if value was in range
                            if (data_in_channel < low_limit || data_in_channel > high_limit)
                                printf("**value out of range");

                        }

                        // printf("\n");
                    }
                    else {
                        printf("Error reading file. %d bytes\n", read);
                        break;
                    }

                } // 	for (i =1; i <= num_samples; i++) {

            } // 	if (size_is_correct) { 
            
        } //  if (header.format_type == 1) { 
        data_file.close();
        fclose(audiofile);
        cout<< endl;
    }
        

    return 0;
}
