Algoritmo genético para averiguar el camino hamiltoniano:
    ·   Camino en un grafo que pase de un nodo a todos y vuelta al origen
    ·   Sin repetir nodos
    ·   Menor coste

Cada individuo contiene:
    ·   Una lista aleatoria de nodos

La función de evaluación comprueba:
    ·   Que el camino sea posible:
        ·   No hay nodos repetidos
        ·   Existen caminos de un nodo al siguiente

Emparejamiento:
    ·   Coger dos individuos aleatorios

Cruce:
    ·   Genera un nuevo individuo a partir de los dos elegidos
        ·   Una tercera parte del camino de uno
        ·   Una tercera parte del camino del otro
        ·   La última parte se genera para que tenga sentido

Mutación:
    ·   Sucede si un número aleatorio generado está bajo un umbral
    ·   Si esto sucede, el hijo muta:
        ·   Intercambiar dos posiciones
        ·   Cada ID + 1 % num nodos

Selección:
    ·   Si la evaluación es mejor que la de los padres

Implementación:
    ·   Se guardan los hijos en la población A en la B.
    ·   Se intercambian los punteros de ambas poblaciones.
    ·   Se guardan los hijos de la población B en la A.
    ...