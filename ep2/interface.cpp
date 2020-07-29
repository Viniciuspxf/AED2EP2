#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
using namespace std;

bool ehSimbolo(char caracter) {

    switch (caracter) {
        case '*':
            return true;
            break;
        case '?':
            return true;
            break;
        case '"':
            return true;
            break;
        case ';':
            return true;
            break;
        case ':':
            return true;
            break;
        case '{':
            return true;
            break;
        case '}':
            return true;
            break;
        case '[':
            return true;
            break;
        case ']':
            return true;
            break;
        case '-':
            return true;
            break;
        case '.':
            return true;
            break;
        case ',':
            return true;
            break;
        case '!':
            return true;
            break;
        case '(':
            return true;
            break;
        case ')':
            return true;
            break;
        case '\357':
            return true;
            break;
        case '\273':
            return true;
            break;
        case '\277':
            return true;
            break;
    }

    return false;
}

class no {
    public:
        string palavra;
        vector<int> listaAdj;
};

//k definido como no enunciado será passado como argumento de linha de comando
class Grafo{
    private:
        int maiorComponente;
        int menorComponente;
        int minLetras;
        int nArestas;
        vector<no> nos;

        bool conectados(string a, string b) {
            int contador = 0, i, j, k, posicoes[3];
            bool conectado = false;
            string aux;
            if (a.size() == b.size()) {
                for (i = 0; i < a.size() && contador != 3; i++) {
                    if (a[i] != b[i]) {
                        posicoes[contador] = i;
                        contador++;
                    }
                }
    
                if (contador == 1) conectado = true;
                if (contador == 2 && a[posicoes[0]] == b[posicoes[1]] && a[posicoes[1]] == b[posicoes[0]])
                    conectado = true;
            }
            else {
                if (a.size() < b.size()) {
                    aux = a;
                    a = b;
                    b = aux;
                }

                if (b.size() == a.size() - 1) {
                    for (i = 0; i < a.size(); i++) {
                        contador = 0;
                        for (j = 0, k = 0; j < b.size(); j++, k++) {
                            if (k != i) contador += (a[k] != b[j]); 
                            else j--;
                        }
                        if (contador == 0) {
                            conectado = true;
                            break;
                        }
                    }
                }
            }
            return conectado;
        }
        
    public:

    Grafo(int k){
        fstream arquivo;
        string nomeArquivo, entrada;
        int diferenca, i;
        cout << "Digite o nome do arquivo: ";
        cin >> nomeArquivo;
        nArestas = 0;
        minLetras = k;
        arquivo.open(nomeArquivo);

        while (arquivo >> entrada) {
            for (i = 0; i < entrada.size() && ehSimbolo(entrada[i]); i++);
            
            entrada.erase(0, i);
            
            for (i = entrada.size(); i > 0 && ehSimbolo(entrada[i - 1]); i--);

            if (i != entrada.size())
                entrada.erase(i, entrada.size() - i);

            diferenca = insere(entrada);

            nArestas += (diferenca > 0 ? diferenca : 0);
        }
        /* Inicializa um grafo com parâmetro k */
    }

    int insere(string palavra){
        int i, j, contador = 0;
        bool achou = false;
        
        if (palavra.size() < minLetras)
            return -1;
        
        for (i = 0; i < nos.size() - achou; i++) {
            
            if (nos[i].palavra == palavra)
                return -1;

            if (conectados(nos[i].palavra, palavra)) {
                for (j = 0; j < nos[i].listaAdj.size() && !achou; j++) {
                    if (nos[nos[i].listaAdj[j]].palavra == palavra)
                        return -1;     
                }
                contador++;
                if (!achou) {
                    nos.push_back({palavra, {}});
                    achou = true;
                }
                
                nos[nos.size() - 1].listaAdj.push_back(i);
                nos[i].listaAdj.push_back(nos.size() - 1);
            }
        }

        if (!achou)
            nos.push_back({palavra, {}});
        return contador;
        /* Insere a palavra e retorna o número de arestas adicionados ao grafo,
        retorna -1 se a palavra já está no grafo ou tem tamanho menor que k*/
    }

    int vertices(){
        return nos.size();
        /* Retorna o número de vértices do grafo*/
    }

    int arestas(){
        return nArestas;
        /* Retorna o número de vértices do grafo*/
    }

    double grauMedio() {
        return ((nos.size() >= 1) ? ((double) 2*nArestas)/nos.size(): 0);
    }

    double densidade() {
        return ((nos.size() > 1) ? ((double) 2*nArestas)/(nos.size()*(nos.size() - 1)) : 0);
    }

    int componentes(){
        int nComp = 0, i, j, atual, tamCompAtual;
        bool *visitado = new bool[nos.size()];
        queue<int> fila;
        maiorComponente = 0;
        menorComponente = 0;

        for (i = 0; i < nos.size(); visitado[i] = false, i++); 
        
        for (i = 0; i < nos.size(); i++) {
            if (!visitado[i]) {
                visitado[i] = true;
                nComp++;
                fila.push(i);
                tamCompAtual = 0;
                while(!fila.empty()) {
                    tamCompAtual++;
                    atual = fila.front();
                    fila.pop();

                    for (j = 0; j < nos[atual].listaAdj.size(); j++) 
                        if (!visitado[nos[atual].listaAdj[j]]) {
                            visitado[nos[atual].listaAdj[j]] = true;
                            fila.push(nos[atual].listaAdj[j]);
                        }
                }
                maiorComponente = (maiorComponente < tamCompAtual ? tamCompAtual : maiorComponente);
                menorComponente = (menorComponente == 0 || menorComponente > tamCompAtual ? tamCompAtual : menorComponente);
            }
        }
        delete [] visitado;
        return nComp;
        /* Retorna o número de componentes do grafo */
    }

    double tamMedComponentes() {
        return (nos.size() != 0 ? ((double) nos.size())/componentes() : 0);
    }

    void maxMinComponentes() {
        componentes();
        cout << "A maior componente tem " << maiorComponente << " vértices." << endl;
        cout << "A menor componente tem " << menorComponente << " vértices." << endl;
    }

    bool conexo(){
        return componentes() <= 1;
        /* Retorna se o grafo é ou não conexo */
    }

    int tamComp(string palavra){
        int i, tam = 0, atual;
        bool *visitado;
        queue<int> fila;

        for (i = 0; i < nos.size() && nos[i].palavra != palavra; i++);

        if (i == nos.size()) return -1;

        visitado = new bool[nos.size()];
        fila.push(i);
        
        for (i = 0; i < nos.size(); visitado[i] = false, i++);

        while (!fila.empty()) {
            tam++;
            atual = fila.front();
            fila.pop();

            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                if (!visitado[nos[atual].listaAdj[i]]) {
                    visitado[nos[atual].listaAdj[i]] = true;
                    fila.push(nos[atual].listaAdj[i]);
                }
            }
        }
        delete [] visitado;
        return tam;
        /* Retorna o tamanha da componente conexa onde está a palavra
        ou -1 caso ela não se encontre no grafo */
    }

    int dist(string a, string b){
        int *dist, i, inicio = -1, fim = -1, atual, distancia;
        dist = new int[nos.size()];
        queue<int> fila;

        for (i = 0; i < nos.size(); dist[i] = -1, i++);

        for (i = 0; i < nos.size(); i++) {
            if (nos[i].palavra == a) inicio = i;
            if (nos[i].palavra == b) fim = i;
        }

        if (inicio == -1 || fim == -1) return -1;

        fila.push(inicio);
        dist[inicio] = 0;

        while (!fila.empty()) {
            atual = fila.front();
            fila.pop();
            
            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                distancia = dist[nos[atual].listaAdj[i]];
                if (distancia == -1 || distancia > dist[atual] + 1) {
                    dist[nos[atual].listaAdj[i]] = dist[atual] + 1;
                    fila.push(nos[atual].listaAdj[i]);
                }
            }
        }

        distancia = dist[fim];
        delete [] dist;
        return distancia;
        /* Retorna a menor distância entre as palavras a e b ou -1
        caso elas estejam desconexas ou não estejam no grafo */
    }

    double mediaDist() {
    int *dist, i, j, atual, distancia, somatoria = 0, total = 0;
        dist = new int[nos.size()];
        queue<int> fila;

        for (j = 0; j < nos.size(); j++) {
            
            for (i = 0; i < nos.size(); dist[i] = -1, i++);

            fila.push(j);
            dist[j] = 0;

            while (!fila.empty()) {
                atual = fila.front();
                fila.pop();
                
                for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                    distancia = dist[nos[atual].listaAdj[i]];
                    if (distancia == -1 || distancia > dist[atual] + 1) {
                        dist[nos[atual].listaAdj[i]] = dist[atual] + 1;
                        fila.push(nos[atual].listaAdj[i]);
                    }
                }  
            }
            
            for (i = 0; i < nos.size(); i++) {
                if (j != i && dist[i] != -1) {
                    somatoria += dist[i];
                    total++;
                }
            }
        }
        delete [] dist;
        return (total != 0 ? ((double) somatoria)/total : 0);
    }

    bool emCiclo(string a){
        queue<int> fila;
        queue<int> pai;
        int i, comeco, *visitado, atual, anterior;

        for (i = 0; i < nos.size() && nos[i].palavra != a; i++)
        
        if (i == nos.size()) return false;

        comeco = i;
        anterior = -1;
        visitado = new int[nos.size()];

        for (i = 0; i < nos.size(); visitado[i] = false, i++);

        fila.push(comeco);
        pai.push(anterior);
        
        while (fila.empty()) {
            atual = fila.front();
            anterior = pai.front();

            fila.pop();
            pai.pop();

            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                if (!visitado[nos[atual].listaAdj[i]]) {
                    visitado[nos[atual].listaAdj[i]] = true;
                    pai.push(atual);
                    fila.push(nos[atual].listaAdj[i]);
                }
                else if (anterior != comeco && nos[nos[atual].listaAdj[i]].palavra == a) {
                    delete [] visitado;
                    return true;
                }
            } 
        }

        delete [] visitado;
        return false;
        /* Retorna verdadeiro casa a palavra esteja em algum ciclo,
        falso caso contrário */
    }

    bool emCiclo(string a, string b) {
        queue<int> fila;
        bool outraPalavra = false;
        int i, comeco, *visitado, atual;

        for (i = 0; i < nos.size() && nos[i].palavra != a; i++)
        
        if (i == nos.size()) return false;

        comeco = i;
        visitado = new int[nos.size()];

        for (i = 0; i < nos.size(); visitado[i] = false, i++);

        fila.push(comeco);
        
        while (fila.empty()) {
            atual = fila.front();
            fila.pop();

            if (nos[atual].palavra == b)
                outraPalavra = true;

            for (i = 0; i < nos[atual].listaAdj.size(); i++) {
                if (!visitado[nos[atual].listaAdj[i]]) {
                    visitado[nos[atual].listaAdj[i]] = true;
                    fila.push(nos[atual].listaAdj[i]);
                }
                else if (outraPalavra && nos[nos[atual].listaAdj[i]].palavra == a) {
                    delete [] visitado;
                    return true;
                }
            } 
        }

        delete [] visitado;
        return false;
        /* Retorna verdadeiro casa exista um ciclo que contenha ambas as palavras,
        falso caso contrário */
    }

    void print() {
        int i, j;

        for (i = 0; i < nos.size(); i++) {
            cout << "palavra: " << nos[i].palavra << "\n\n";
            
            for (j = 0; j < nos[i].listaAdj.size(); j++) {
                cout << nos[nos[i].listaAdj[j]].palavra << endl;
            }
            cout << "\n\n\n\n";
        }
    }

};

//observação, as duas palavras precisam ser diferentes na função do ciclo