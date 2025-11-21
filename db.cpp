#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

struct InputBuffer{
    string buffer;
    ssize_t input_length;
};

InputBuffer* new_input_buffer(){
    InputBuffer* a_new_buffer = new InputBuffer();
    return a_new_buffer;
};

void print_prompt(){
    cout<<"db > ";
    return;
}


void read_input(InputBuffer* reader){
    getline(cin,reader->buffer);
    int content_size = reader->buffer.size();
    reader->input_length = content_size;
    return;
}

void close_input_buffer(InputBuffer* reader){
     delete reader;
     return;
}


int main (){
    InputBuffer* input_buffer = new_input_buffer();
    string user_data = "";
    while(true){
        print_prompt();
        read_input(input_buffer);
        user_data += input_buffer->buffer;
        if(input_buffer->buffer == ".exit"){
            close_input_buffer(input_buffer);
            break;
        }
    }

    cout<<"User text  - "<<endl;
    cout<<user_data<<endl;

    return 0;


}

    