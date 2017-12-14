# Gu&iacute;a Git
### Para un correcto funcionamiento del Proyecto:

* Cada miembro tendrá su propia rama (Preferiblemente llamada como él)
* Procedimiento a llevar cada sesión de trabajo individual:
  1. Nos aseguramos de que la rama local principal _master_ está actualizada con el repositorio remoto **(git pull)**
  2. Creamos la rama en la que vamos a trabajar nombrada como nosotros con **git checkout -b nombre_rama**
  3. La linkeamos con _master_ mediante **git push --set-upstream origin nombre_rama**
  4. Entramos a branches en el Repositorio de GitHub, si nuestra rama va a la par que master, ya estamos listos para trabajar.
  5. Añadimos todos los ficheros modificados usando **git add f_mod_1 f_mod_2 ... f_mod_n** 
  (Podemos abreviar este paso si tenemos todos los archivos guardados en una carpeta) **git add nombre_carpeta**
  6. Guardamos los cambios en comits **git commit -m "Nombre del commit"** 
  7. Una vez acabada la sesión **git push origin nombre_rama** para subir los commits
  8. Entramos al Repositorio en GitHub y vamos a branches. Hacemos un pull request de nuestra rama, y si no da problemas la mergeamos
  9. Por último, volvemos a Git y borramos la rama **git branch -d nombre_rama** desde la rama master **(git checkout master)**.
  Una vez borrada localmente, la borramos de GitHub.
  
>### Si hacemos esto cada vez que trabajemos nos aseguramos de que no vamos a interferir con el trabajo de los demás.
