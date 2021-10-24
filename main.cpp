/* Trabalho PCO001: Compressão de dados — Huffman
 *
 * 2021101312	Alexandre Leles Sousa
 * 2021101359	Daniel Paiva Fernandes
 * 2021101466	Natalia Sanchez Sanchez
 *
*/

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

class Huffman {
    struct NovoNode {
        char dado;
        size_t freq;
        NovoNode *esquerdo;
        NovoNode *direito;

        NovoNode(char dado, size_t freq) : dado(dado),
                                           freq(freq),
                                           esquerdo(nullptr),
                                           direito(nullptr) {}

        ~NovoNode() {
            delete esquerdo;
            delete direito;
        }
    };

    struct compara {
        bool operator()(NovoNode *l, NovoNode *r) {
            return (l->freq > r->freq);
        }
    };

    NovoNode *topo{};

    void imprimeCodigo(string &frase, NovoNode *raiz, const string &basicString) {
        if (raiz == nullptr)
            return;

        if (raiz->dado == '$') {
            imprimeCodigo(frase, raiz->esquerdo, basicString + "0");
            imprimeCodigo(frase, raiz->direito, basicString + "1");
        }

        if (raiz->dado != '$') {
            // Substitui na cópia da frase o caractere pela cadeia de bits
            string old_value;
            old_value.push_back(raiz->dado);
            size_t pos;
            while ((pos = frase.find(old_value)) != std::string::npos) {
                frase.replace(pos, 1, basicString);
            }

            // Imprime o valor em bits para o respectivo caractere
            cout << raiz->dado << " : " << basicString << "\n";
            imprimeCodigo(frase, raiz->esquerdo, basicString + "0");
            imprimeCodigo(frase, raiz->direito, basicString + "1");
        }
    }

public:
    Huffman() = default;;

    ~Huffman() {
        delete topo;
    }

    string GeraArvoreHuffman(string frase, vector<char> &dado, vector<size_t> &freq) {
        NovoNode *esquerdo;
        NovoNode *direito;
        string fraseCodificada = std::move(frase);
        priority_queue < NovoNode * , vector<NovoNode *>, compara > minHeap;
        size_t tamanho = dado.size();

        for (size_t i = 0; i < tamanho; ++i) {
            minHeap.push(new NovoNode(dado[i], freq[i]));
        }

        while (minHeap.size() != 1) {
            esquerdo = minHeap.top();
            minHeap.pop();

            direito = minHeap.top();
            minHeap.pop();

            topo = new NovoNode('$', esquerdo->freq + direito->freq);
            topo->esquerdo = esquerdo;
            topo->direito = direito;
            minHeap.push(topo);
        }
        imprimeCodigo(fraseCodificada, minHeap.top(), "");
        return fraseCodificada;
    }

    static long TaxaDeCompressao(const string &original, const string &comprimidoEmBits) {
        return 100 - ((comprimidoEmBits.size() + 1) * 100 / ((original.size() + 1) * 8));
    }
};

vector<char> &converteStringEmVetor(string &frase, vector<char> &caracteres,
                                    vector<size_t> &freq) {
    //Adiciona a string dentro do vetor de caracteres
    copy(frase.begin(), frase.end(), back_inserter(caracteres));
    //Ordena os caracteres dentro do vetor
    sort(caracteres.begin(), caracteres.end());
    //Remove entradas duplicadas
    caracteres.erase(unique(caracteres.begin(), caracteres.end()), caracteres.end());
    //Conta o número de ocorrências na frase e adiciona à frequência
    for (char &caractere: caracteres) {
        int frequencia = count(frase.begin(), frase.end(), caractere);
        freq.insert(freq.end(), frequencia);
    }
    return caracteres;
}

/**
 *  Implemente a codificação de Huffman usando a linguagem C/C++ e o paradigma Guloso.
 *  Escreva a cadeia de bits que codificação a string <O tempo perguntou pro tempo quanto
 *  tempo o tempo tem. O tempo respondeu pro tempo que o tempo tem tanto tempo quanto
 *  tempo o tempo tem>. Responda:
 *  a) Qual é a tabela de codificação?
 *  b) Qual a codificação obtida para a dada string?
 *  c) Quantos bits tem a cadeia?
 *  d) Qual a taxa de compressão?
 */
int main() {
    Huffman huffman;
    vector<char> caracteres;
    vector<size_t> freq;
    std::string frase = "O tempo perguntou pro tempo quanto tempo o tempo tem. "
                        "O tempo respondeu pro tempo que o tempo tem tanto tempo quanto tempo o tempo tem";

    caracteres = converteStringEmVetor(frase, caracteres, freq);

    cout << "a) Qual é a tabela de codificação?" << endl;
    string cadeia = huffman.GeraArvoreHuffman(frase, caracteres, freq);

    cout << "b) Qual a codificação obtida para a dada string?" << endl
         << cadeia << endl;

    cout << endl << "c) Quantos bits tem a cadeia?" << endl
         << "Cadeia antiga: " + std::to_string((frase.size() + 1) * 8) << endl
         << "Cadeia nova: " + std::to_string(cadeia.size() + 1) << endl;

    long compressao = Huffman::TaxaDeCompressao(frase, cadeia);
    cout << endl << "d) Qual a taxa de compressão?" << endl
         << "Compressão de " << std::to_string(compressao) << "%" << endl;

    return 0;
}

	