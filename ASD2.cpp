#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <fstream>


#define MAX_TREE_HT 100
#define EMPTY_STRING ""
#define SIZE 26
using namespace std;

fstream input;
fstream output;
string result;

// Struktura z symbolem, jego częstotliwością i węzłem
struct Node
{
    char ch;
    int freq;
    Node* left, * right;
};

// Funkcja przydzielająca nowy liść
Node* getNode(char ch, int freq, Node* left, Node* right)
{
    Node* node = new Node();

    // wskazujemy wartości i kierunki

    node->ch = ch;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

// Porównanie obiektów za pomocą którego układamy stos
struct comp
{
    bool operator()(const Node* l, const Node* r) const
    {
        // pozycja o najwyższym priorytecie ma najniższą częstotliwość
        return l->freq > r->freq;
    }
};

// Funckja do sprawdzania czy drzewo zawiera tylko jeden węzeł jeżeli korzeń to wierzchołek
bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

// Przeszukiwanie drzewa i przechowywanie kodów w mapie
void encode(Node* root, string str, unordered_map<char, string>& huffmanCode)
{
    // jeżeli korzeń
    if (root == nullptr) {
        return;
    }

    // jeżeli liść
    if (isLeaf(root)) {
        huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// Funkcja budująca drzewo
void buildHuffmanTree(string text)
{
    // podstawa : pusty string
    if (text == EMPTY_STRING) {
        return;
    }

    // zliczanie częstotliwości symboli
    // i przechowywanie ich w mapie
    unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    // tworzenie kolejki priorytetowej do przechowywania węzłów
    priority_queue<Node*, vector<Node*>, comp> pq;

    // tworzenie wierzchołka dla każdego symbolu
    // i dodanie go do kolejki priorytetowej
    for (auto pair : freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    // do, dopóki w kolejce nie będzie więcej niż jeden węzeł
    while (pq.size() != 1)
    {
        // Usuń dwa węzły o najwyższym priorytecie
        // (najniższa częstotliwość) z kolejki

        Node* left = pq.top(); pq.pop();
        Node* right = pq.top();    pq.pop();

        // utwórz nowy węzeł wewnętrzny z tymi dwoma węzłami jako dziećmi i
        // z częstotliwością równą sumie częstotliwości dwóch węzłów
        // Dodaj nowy węzeł do kolejki priorytetowej

        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    // wskazanie na korzeń drzewa
    Node* root = pq.top();

    // Przenoszenie drzewa i przechowywanie kodów w mapie
    // oraz wpychanie par (symbol, tłumaczenie) do stringa
    unordered_map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    string dictionary = "Slownik:";
    result.append(dictionary);

    string coded;
    for (auto pair : huffmanCode) {
        coded.push_back(pair.first);
        coded.push_back(' ');
        coded.append(pair.second);
        coded.push_back('\n');
    }
    result.push_back('\n\n');
    result.append(coded);

    string str;
    for (char ch : text) {
        str += huffmanCode[ch];
    }
    string beautify = "Zakodowany string:";

    result.push_back('\n');
    result.append(beautify);
    result.push_back('\n');
    result.append(str);
}
int main()
{
    string str;
    input.open("stringFile.txt", istream::in);

    getline(input, str);

    input.close();

    cout << str;

    buildHuffmanTree(str);

    output.open("codingResult.txt", ostream::out);
    output << result;
    output.close();

	return 0;
}
