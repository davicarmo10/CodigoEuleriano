#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

#define FIN_CICLO -1

int **leerMatrizAdyacencia(char *nombreArchivo)
{    
    ifstream archivo(nombreArchivo);
    
    if (archivo == NULL) return NULL;
    
    int **matriz;
    
    int n;

    archivo>>n;
    
    matriz = new int*[n];
    
    for(int i=0; i<n; i++)
    {
            matriz[i] = new int[n];
            
            for(int j=0; j<n; j++)
                    archivo>>matriz[i][j];        
    }
    
    return matriz;
}

void mostrarMatrizAdyacencia(int **matriz, int n)
{
     for (int i=0; i<n; i++)
         {
              for (int j=0; j<n; j++)
                  cout<<matriz[i][j]<<" ";
              cout<<"\n";     
         }
         
     cout<<"\n";
}

void destruirMatrizAdyacencia(int **matriz, int n)
{
     for (int i=0; i<n; i++)
         delete matriz[i];
         
     delete matriz;
}

int dentroDeCiclo(int *ciclo, int vertice)
{
    int enCiclo = false;
         
    int k = 0;
         
    while (ciclo[k] != FIN_CICLO && !enCiclo)
    {
          if (vertice == ciclo[k]) enCiclo = true;
           
          k++;
    }
    
    return enCiclo;
}

void mostrarCiclo(int *ciclo)
{    
     for (int i=0; ciclo[i]!=FIN_CICLO; i++)
        {
            cout<<ciclo[i]<<" ";
        }
        
     cout<<"\n";
}

int *encontrarCiclo(int **matriz, int vertice, int n)
{
     int v;
     
     int *ciclo = new int[n*n + 1];
     
     ciclo[0] = FIN_CICLO;
     
     v = vertice;
     
     int i = 0;
     
     do
     {
         int j = 0;
                  
         while (matriz[v][j] == 0 || dentroDeCiclo(ciclo, j))
         {
               j++;
             
               while (j == n)
               {
                     
                  i--;
                   
                  if (i < 0) return NULL;
                           
                  else if (i == 0)
                  {
                      v = vertice;  
                  }
                  else
                  {
                      v = ciclo[i-1];
                  }
                           
                  j = ciclo[i] + 1;

                  ciclo[i] = FIN_CICLO;
                  
                  mostrarCiclo(ciclo);
               }
         }
                            
         v = j;
              
         ciclo[i] = v;
         
         ciclo[i+1] = FIN_CICLO;
     
         i++;
         
         mostrarCiclo(ciclo);
         
         if (i >= n*n) return NULL;
         
     }while(v != vertice || i<=2);
          
     return ciclo;
}

void eliminarAristas(int **matriz, int *ciclo)
{
    int i;
    
    if (ciclo[0] == FIN_CICLO) return;
        
    i = 1;
    
    while (ciclo[i] != FIN_CICLO)
    {
          
          matriz[ciclo[i-1]][ciclo[i]] = 0;
          matriz[ciclo[i]][ciclo[i-1]] = 0;
          
          
          i++;
    }
    
    matriz[ciclo[i-1]][ciclo[0]] = 0;
    matriz[ciclo[0]][ciclo[i-1]] = 0;
}

int tieneAristas(int **matriz, int n)
{
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            if (matriz[i][j] == 1) return true;
            
    return false;
}

void pegarCiclos(int *ciclo, int *subciclo, int posicion)
{
     int i, tamano_ciclo, tamano_subciclo;
     
     for (i = 0; ciclo[i] != FIN_CICLO; i++);
     
     tamano_ciclo = i;
     
     for (i = 0; subciclo[i] != FIN_CICLO; i++);
     
     tamano_subciclo = i;
     
     int *ciclo_aux = new int[tamano_ciclo + tamano_subciclo + 1];
     
     for (i = 0; i<posicion; i++)
         ciclo_aux[i] = ciclo[i];
         
     
     for (i = posicion; i< posicion + tamano_subciclo; i++)
         ciclo_aux[i] = subciclo[i - posicion];
         
     
     for (i = posicion + tamano_subciclo; i < tamano_ciclo + tamano_subciclo; i++)
         ciclo_aux[i] = ciclo[i - tamano_subciclo];
     
     
     for (i = 0; i <  tamano_ciclo + tamano_subciclo; i++)
         ciclo[i] = ciclo_aux[i];
     
     ciclo[tamano_ciclo + tamano_subciclo] = FIN_CICLO;
     
     delete ciclo_aux;
}

int main(int argc, char *argv[])
{
    
    int **matriz = leerMatrizAdyacencia("matriz.txt");
    
    if (matriz == NULL)
       {
               cout<<"El archivo no existe\n";
               
               system("PAUSE");
               
               return 0;
       }

    cout << "matriz de adyacencia inicial: \n";

    mostrarMatrizAdyacencia(matriz, 8);
    
    int *ciclo = encontrarCiclo(matriz, 3, 8);
    
    if (ciclo == NULL)
    {
              cout<<"no hay ciclo\n";
              
              return 0;
    }
    
    cout <<"ciclo inicial: \n";
    
    mostrarCiclo(ciclo);
    
    eliminarAristas(matriz, ciclo);
        
    int i, *subciclo, *euleriano  = ciclo;
    
    while (tieneAristas(matriz, 8))
    {
          cout << "matriz de adyacencia sin las aristas del ciclo euleriano: \n";
              
          mostrarMatrizAdyacencia(matriz, 8);

          cout<<"ciclo euleriano actual:\n";
    
          mostrarCiclo(euleriano);
          
          system("PAUSE");
          
          subciclo = NULL;
          
          i = 0;
          
          do
          {
              
              subciclo = encontrarCiclo(matriz, euleriano[i], 8);
              
              i++;
          
          }while(subciclo==NULL && euleriano[i] != FIN_CICLO);
          
          
          if (subciclo == NULL) 
             cout<<"no hay subciclo";
          else
          {
              cout<<"subciclo encontrado:\n";
          
              mostrarCiclo(subciclo);
          
              pegarCiclos(euleriano, subciclo, i);
              
              cout<<"insertando subciclo en la posicion "<<i<<" del ciclo euleriano:\n";
    
              mostrarCiclo(euleriano);
          
              system("PAUSE");
          
              eliminarAristas(matriz, subciclo);
          }
          
    }
    
    
    cout<<"ciclo euleriano final:\n";
    
    mostrarCiclo(euleriano);

    
    destruirMatrizAdyacencia(matriz, 8);    
    
    system("PAUSE");
    return EXIT_SUCCESS;
}
