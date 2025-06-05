#include<iostream>
#include<vector>
#include<unordered_map>
#include<queue>
#include <fstream>
using namespace std;
// compressions is the unique mapping relatuve to freq of chracters 
// relative freq and uniquen mapping 
class Node{
public:
    int freq;
    char data;
    Node* left;
    Node* right;
    Node(int freq, char data){
        this->freq = freq;
        this->data = data;
        this->left = NULL;
        this->right = NULL;
    }
    bool operator()(const Node* a, const Node* b){
        return a->freq > b->freq; // max heap
    }
};
void serialize_dfs(Node* root,string& s){
    if(root==NULL){
        s+=",#";
        return ;
    }
    s+=(","+to_string(root->data));
    if(root->left==NULL && root->right!=NULL){
        s+=",#";
        serialize_dfs(root->right,s);
    }else if(root->left!=NULL && root->right==NULL){
        serialize_dfs(root->left,s);
        s+=",#";
    }else{
        serialize_dfs(root->left,s);
        serialize_dfs(root->right,s);
    }
    return ;
}
string serialize(Node* root) {
    string ans="";
    serialize_dfs(root,ans);
    return ans;
}
void DFS(Node* node, string str,unordered_map<char,string>& compressions){
    if(node->left==NULL && node->right==NULL){
        compressions[node->data] = str;
        return ;
    }
    DFS(node->left, str+"0",compressions);
    DFS(node->right, str+"1",compressions);
    return ;
}
string encoding(string s){
    unordered_map<char, int> freq;
    unordered_map<char,string> compressions;
    for(auto i:s){
        freq[i]++;
    }
    // create min heap buffers
    priority_queue<Node*> pq;
    for(auto i:freq){
        pq.push(new Node(i.second, i.first));
    }
    while(pq.size()>1){
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* parent = new Node(left->freq+right->freq, '#');
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    Node* root = pq.top();
    DFS(root,"",compressions);
    string compressed_string = "";
    for(auto i:s){
        compressed_string += compressions[i];
    }
    // till now is fine now we map each 8 bits in string to correponding ASCII characters 
    // then we can decode it
    // padding length to multiple of 8 bits
    int padding =(8 - compressed_string.length()%8)%8; // padding length to multiple of 8 bits
    for(int i=0;i<padding;i++){
        compressed_string += "1";
    }
    string compressed_string_final = "";
    // converting each 8 bits to ASCII characters 8 bits at a time
    for(int i=0;i<compressed_string.length();i+=8){
        int num = 0;
        for(int j=0;j<8;j++){
            num = num*2 + (compressed_string[i+j]-'0'); // converting binary to decimal
        }
        compressed_string_final += char(num);
    }
    cout<<"Encoded string: \n\n"<<compressed_string_final<<"\n\n"<<endl;
    return serialize(root)+ "\n" + to_string(padding) +"\n"+compressed_string_final ;
}

// decompression function
void dfs(Node* node, string str,unordered_map<string,char>& decompose){
    if(node->left==NULL && node->right==NULL){
        decompose[str] = node->data;
        return ;
    }
    dfs(node->left, str+"0",decompose);
    dfs(node->right, str+"1",decompose);
    return ;
}
Node* deserialize_dfs(string s,int& i){
    if(s[i]==',' && s[i+1]=='#'){
        i+=2;
        return NULL;
    }
    i+=1;
    string num="";
    while(s[i]!=','){
        num+=s[i];
        i++;
    }
    Node* root=new Node(0,stoi(num));
    root->left=deserialize_dfs(s,i);
    root->right=deserialize_dfs(s,i);
    return root;
}
    // Decodes your encoded data to tree.
Node* deserialize(string data) {
    int i=0;
    return deserialize_dfs(data,i);
}
string decoding(string compressed_string){
    string data1=compressed_string.substr(0,compressed_string.find("\n"));
    compressed_string = compressed_string.substr(compressed_string.find("\n")+1);
    string data2 = (compressed_string.substr(0,compressed_string.find("\n")));
    compressed_string = compressed_string.substr(compressed_string.find("\n")+1);
    // converting compressed string to binary
    // convert ASCII characters back to 8 bits binary
    string compressed_string_final = "";
    unordered_map<string,char> decompose;
    Node* root=deserialize(data1);
    dfs(root,"",decompose);
    for(auto i:compressed_string){
        int num = int(i);
        // for negative numbers convert to positive and add 256 to get original number 8 bits binary form.
        if(num<0) num=256+num; // for negative numbers convert to positive and add 256 to get original number 8 bits binary form.
        string binary = "";
        for(int j=0;j<8;j++){
            binary = to_string(num%2) + binary;
            num=num/2;
        }
        compressed_string_final += binary;
    }
    // remove padding
    compressed_string_final = compressed_string_final.substr(0,compressed_string_final.length()-stoi(data2));
    int i=0;
    string decoded_string = "";
    while(i<compressed_string_final.length()){
        string temp = "";
        while(i<compressed_string_final.length() && decompose.find(temp)==decompose.end()){
            temp += compressed_string_final[i];
            i++;
        }
        decoded_string += decompose[temp];

    }
    return decoded_string;
}
int main(){
    string s;
    cout<<"\n\nEnter the string you want to encode and compress: \n\n";
    getline(cin,s); // if you want to input string from file
    // getline(cin,s);
    string line;
    cout<<"\n\n"<<endl;
    cout<<"Input string: \n\n"<<s<<"\n\n"<<endl;
    string Encoded_string = encoding(s);
    string decoded_string = decoding(Encoded_string);
    // cout<<decoded_string<<endl;
    cout<<"Decoded string: \n\n"<<decoded_string<<"\n\n"<<endl;
    return 0;
 
}