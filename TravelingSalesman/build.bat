rd /s /q tsp
javac -sourcepath src src/tsp/Main.java -d src/..
jar cfe tsp.jar tsp.Main tsp/*.class
pause