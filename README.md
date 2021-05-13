# Desafio-3-EDAS
algoritmo de detección de colisiones hecho en c++.

<h1>AABB tree Collision detección</h1>

<h3>Instrucciones de ejecución</h3>

- Ingresar al link https://codecollab.io/@cristoferfb/Desafio%202
- Ya en la pagina pedira un nombre de usuario, poner cualquiera.
- Estando ya dentro del compilador clickear el archivo main.cpp.
- Teniendo el archivo seleccionado clickear el boton ejecutar.

<h3>Descripción del problema</h3>

  En los inicios de los video juegos, la mayoria se componian de figuras bidimensionales, las cuales interacctuaban por la posicion de sus puntos en el plano cartesiano comun, esto se ve representado de mejor manera en un juego que todos conocemos "space invader" en el cual solo interacctuaban entre si los disparos de la nave o de los invasores con el contrario y con los muros que nos cubrian de esos temibles disparos 2D, Con el reciente desarrollo de la industria del video juego, cada vez mas se va haciendo mas presente el modelado 3D de los personajes, mapas, armas, etc.., en los cuales es predominantemente dificil saber cuando un objeto interactua con otro, lo que la mayoria de los gamers conocemos como "Hitbox" o caja de colision, pero realmente no sabemos como funciona internamente esta "Hitbox", para esto definamoslo un poco mas en concreto:

  Una hitbox es una técnica invisible comúnmente utilizada en los videojuegos para la detección de colisiones en tiempo real, también llamadas cajas delimitadoras o cajas de colisión. A menudo es un rectángulo (en videojuegos 2D) o un ortoedro (en videojuegos 3D) que se añade y sigue un punto de un objeto visible (como un modelo o un sprite). También es común usar formas elípticas y esferoides, aunque se les suele llamar igualmente "cajas". Es habitual que los objetos animados tengan cajas de colisión (hitboxes) unidas a cada parte móvil para asegurar la precisión durante su movimiento.

  Las cajas delimitadoras o cajas de colisión (hitboxes) se utilizan para detectar colisiones en una única dirección, como un personaje siendo golpeado por un puñetazo o una bala. No son adecuadas para la detección de colisiones interactivas (por ejemplo, colisionar contra una pared) debido a la dificultad que experimentan tanto los humanos como las IAs para gestionar las ubicaciones siempre cambiantes de estas cajas delimitadoras; este tipo de colisiones, por norma general, se gestionan con cajas delimitadores mucho más simples alineadas con los ejes. Aun así algunos jugadores suelen usar el término "hitbox" para referirse a este tipo de interacciones.

  Entonces teniendo esto claro nos damos cuenta que existe una creciente complejidad en el problema, para esto nos ayudaremos de un AABB tree el cual es una estructura de datos de subdivisión espacial y un conjunto de algoritmos usados principalmente para realizar intersecciones y calcular distancias de forma eficiente en objetos geométricos en 3D con un número finito de puntos, la ventaja de esta estructura sobre otras en este problema es como permite realizar busquedas de colisiones de una manera rapida y eficiente a travez de todo el arbol, esto se profundizara mejor en la solución del problema.

<h3>solución propuesta</h3>

Se propone la utlización de la estructura espacial AABB (Axis-Aligned Bounding Box), la cual tiene como finalidad contener a un objeto de cualquier tipo, es decir, la figura en cuestión estará inscrita en la caja. Estas cajas deberán estar alineadas por medio de sus ejes ([x, y] en 2D y [x,y,z] en 3D). Las cajas se podrán ubicar en el espacio con tán sólo 2 puntos y para facilitar esta representación, se pueden tomar puntos de vertices los cuales formen una diagonal. Ahora, en el momento en que dos cajas se solapen habrá una colisión, lo cual se puede verificar comparando los puntos de las cajas.
Para la organización de cajas se propone la utilización de un árbol AABB el cual tiene como finalidad jerarquizar y ordenar las cajas.
	-La raiz del árbol puede ser una rama o una hoja.
	-Una rama puede tener exactamente dos hijos (left y right) y se le asigna un AABB el cual contiene ambos 		descendientes.
	-Una hoja estará asociado con un objecto del espacio y un AABB.
	
La construcción de un árbol es la siguiente:
	-Al estar el árbol vacío, el primero nodo a insertar será una hoja y la raiz. Este nodo será un AABB con el objeto 1 contenido en él.
	-Si queremos añadir otro objeto, primer se creará un nodo rama y se asignará un AABB lo suficientemente grande para contener ambos objetos.
	-Se creará un nuevo nodo hoja para el objeto 2 y se le asigna como descendiente del nuevo nodo rama.
	-Ahora, el nodo hoja original del objeto 1 será descendiente del nuevo nodo rama.
	-Finalmente el nuevo nodo rama será la raiz del árbol.
	
Una duda que surgue es cómo se encuentra la mejor hoja en el arbol para emparejar un nuevo AABB a que debe ingresar. Esto se puede resolver utilizando una heurística para escojar donde se colocará el AABB.

Mediante está estructura y construcción resulta muy eficiente la consulta sobre posibles colisiones que hayan en el árbol con un nuevo AABB. Lo único que se debe hacer, partiendo de la raiz, es verificar si el nodo actual intersecta al nuevo AABB:
	-Si es cierto y es un nodo hoja entonces es una colisión
	-Si también se cumple la colisión y es un nodo rama entonces se descenderá recursivamente a los hijos (left y right) del nodo del árbol.

Otra forma sin ocupar recursión, como normalmente se ocupa cuando tratamos con árboles, es utilizar una pila.

<h3>Cooevaluación</h3>
https://drive.google.com/file/d/1692rHD27tcl_bgEZp6mNSdHVpuSLpWNE/view?usp=sharing

<h3>Video Presentación</h3>
https://drive.google.com/file/d/1nObFm1CwmYlRQJnK2SCXy2Ov5fQ2s_qp/view?usp=sharing
