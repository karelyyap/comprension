#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <set>
#include <cstring>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string modo;
    if (!(cin >> modo)) return 0;

    if (modo == "compress") {
        int n, m;
        cin >> n >> m;
        int total_pixeles = n * m;
        
        vector<unsigned int> img;
        img.reserve(total_pixeles);
        
        set<unsigned int> colores_unicos;
        
        // 1. Leer toda la imagen
        for (int i = 0; i < total_pixeles; i++) {
            string hex_str;
            cin >> hex_str;
            unsigned int color = stoul(hex_str, nullptr, 16);
            img.push_back(color);
            colores_unicos.insert(color);
        }
        
        vector<unsigned int> diccionario_base(colores_unicos.begin(), colores_unicos.end());
        map<unsigned int, int> mapa_base;
        for (size_t i = 0; i < diccionario_base.size(); i++) {
            mapa_base[diccionario_base[i]] = i;
        }
        
        // 2. Compresión LZW con diccionario más eficiente
        map<pair<int, unsigned int>, int> dict_lzw;
        int proximo_codigo = diccionario_base.size();
        vector<int> codigos_comprimidos;
        codigos_comprimidos.reserve(total_pixeles / 2);
        
        const int MAX_DICT_SIZE = 65536;
        
        if (total_pixeles > 0) {
            int codigo_actual = mapa_base[img[0]];
            
            for (int i = 1; i < total_pixeles; i++) {
                unsigned int siguiente_color = img[i];
                auto clave = make_pair(codigo_actual, siguiente_color);
                
                if (dict_lzw.count(clave)) {
                    codigo_actual = dict_lzw[clave];
                } else {
                    codigos_comprimidos.push_back(codigo_actual);
                    
                    if (proximo_codigo < MAX_DICT_SIZE) {
                        dict_lzw[clave] = proximo_codigo++;
                    }
                    
                    codigo_actual = mapa_base[siguiente_color];
                }
            }
            codigos_comprimidos.push_back(codigo_actual);
        }
        
        // 3. Salida
        vector<unsigned int> salida_final;
        salida_final.push_back(diccionario_base.size());
        for (unsigned int color : diccionario_base) {
            salida_final.push_back(color);
        }
        salida_final.push_back(codigos_comprimidos.size());
        for (int codigo : codigos_comprimidos) {
            salida_final.push_back(codigo);
        }
        
        cout << salida_final.size() << "\n";
        for (size_t i = 0; i < salida_final.size(); i++) {
            cout << salida_final[i] << (i == salida_final.size() - 1 ? "" : " ");
        }
        cout << "\n";
        
    } else if (modo == "decompress") {
        int n, m, k;
        cin >> n >> m >> k;
        
        // 1. Reconstruir diccionario base
        int num_base;
        cin >> num_base;
        vector<unsigned int> diccionario_base(num_base);
        for (int i = 0; i < num_base; i++) {
            cin >> diccionario_base[i];
        }
        
        // 2. Leer códigos comprimidos
        int num_codigos;
        cin >> num_codigos;
        vector<int> codigos(num_codigos);
        for (int i = 0; i < num_codigos; i++) {
            cin >> codigos[i];
        }
        
        // Inicializar diccionario de descompresión
        vector<vector<unsigned int>> dict_lzw;
        dict_lzw.reserve(65536);
        
        for (int i = 0; i < num_base; i++) {
            dict_lzw.push_back({diccionario_base[i]});
        }
        
        int pixeles_impresos = 0;
        
        auto imprimir_pixel = [&](unsigned int color) {
            cout << hex << uppercase << setfill('0') << setw(8) << color;
            pixeles_impresos++;
            if (pixeles_impresos % m == 0) cout << "\n";
            else cout << " ";
        };
        
        // 3. Descompresión
        if (num_codigos > 0) {
            int codigo_anterior = codigos[0];
            for (unsigned int color : dict_lzw[codigo_anterior]) {
                imprimir_pixel(color);
            }
            
            for (int i = 1; i < num_codigos; i++) {
                int codigo_actual = codigos[i];
                vector<unsigned int> entrada;
                
                if (codigo_actual < (int)dict_lzw.size()) {
                    entrada = dict_lzw[codigo_actual];
                } else {
                    entrada = dict_lzw[codigo_anterior];
                    entrada.push_back(dict_lzw[codigo_anterior][0]);
                }
                
                for (unsigned int color : entrada) {
                    imprimir_pixel(color);
                }
                
                // Agregar nuevo patrón al diccionario
                if (dict_lzw.size() < 65536) {
                    vector<unsigned int> nueva_entrada = dict_lzw[codigo_anterior];
                    nueva_entrada.push_back(entrada[0]);
                    dict_lzw.push_back(nueva_entrada);
                }
                
                codigo_anterior = codigo_actual;
            }
        }
    }
    return 0;
}
