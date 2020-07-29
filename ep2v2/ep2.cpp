#include "interface.cpp"

int main() {
    int k, opcao, valor;
    string palavra, outraPalvra;
    cout << "Digite o número mínimo de letras de cada palavra no grafo: ";
    cin >> k;
    Grafo grafo(k);

    while (true) {
        cout << "Digite 1 para o número de vértices\n";
        cout << "Digite 2 para o número de arestas\n";
        cout << "Digite 3 para grau médio de vértices\n";
        cout << "Digite 4 para a densidade do grafo\n";
        cout << "Digite 5 para saber se o grafo é conexo\n";
        cout << "Digite 6 para saber quantas componentes o grafo tem\n";
        cout << "Digite 7 para saber o tamanho da componente que uma palavra está\n";
        cout << "Digite 8 para o tamanho médio das componentes\n";
        cout << "Digite 9 para saber a componente máxima e mínima\n";
        cout << "Digite 10 para saber se uma palavra está em um ciclo\n";
        cout << "Digite 11 para saber se se duas palavras estão em um ciclo\n";
        cout << "Digite 12 para saber a distância entre uma palavra e outra\n";
        cout << "Digite 13 para saber a distância média entre uma palavra e outra\n";
        cout << "Digite 14 para sair\n";
        cin >> opcao;
        switch (opcao) {
            case 1:
                cout << "O grafo tem " << grafo.vertices() << " vértices.\n";
                break;
            case 2:
                cout << "O grafo tem " << grafo.arestas() << " arestas.\n";
                break;
            case 3:
                cout << "O grau médio dos vértices é " << grafo.grauMedio() << ".\n";
                break;
            case 4:
                cout << "A densidade do grafo é " << grafo.densidade() << ".\n";
                break;
            case 5:
                if (grafo.conexo()) cout << "O grafo é conexo.\n";
                else cout << "O grafo não é conexo.\n";
                break;
            case 6:
                cout << "O grafo tem " << grafo.componentes() << " componentes.\n";
                break;
            case 7:
                cout << "Digite a palavra: ";
                cin >> palavra;
                valor = grafo.tamComp(palavra);
                if (valor == -1) 
                    cout << "A palavra '"<< palavra << "' não está no grafo. \n";
                else 
                    cout << "A componente com a palavra '" << palavra << "' tem " << valor << " vértices.\n";
                break;
            case 8:
                cout << "O tamanho médio das componentes é " << grafo.tamMedComponentes() << ".\n";
                break;
            case 9:
                grafo.maxMinComponentes();
                break;
            case 10:
                cout << "Digite a palavra: ";
                cin >> palavra;
                cout << "A palavra '" << palavra << "'" << (grafo.emCiclo(palavra)? "": " não") << " está em um ciclo.\n";
                break;
            case 11:
                cout << "Digite as palavras: ";
                cin >> palavra >> outraPalvra;
                cout << "As palavras '" << palavra << "' e '" << outraPalvra << "'" << (grafo.emCiclo(palavra, outraPalvra)? "": " não") << " estão em um ciclo.\n";
                break;
            case 12:
                cout << "Digite as palavras: ";
                cin >> palavra >> outraPalvra;
                valor = grafo.dist(palavra, outraPalvra);
                if (valor == -1)
                    cout << "Não existe caminho de '" << palavra << "' até '" << outraPalvra  << "'.\n";
                else 
                    cout << "A distância entre '" << palavra << "' até '" << outraPalvra << "' é " << valor << ".\n";
                break;
            case 13:
                cout << "A distância média entre uma palavra e outra é " << grafo.mediaDist() << ".\n";
                break;
            case 14:
                cout << "Finalizando...\n";
                break;
            case 100:
                grafo.print();
                break;
            case 101:
                grafo.printDistancias();
                break;
            case 102:
                grafo.printCiclos();
                break;
            default:
                cout << "Comando não reconhecido.\n";
                break; 
        } 
        if (opcao == 14)
            break;
    }
}