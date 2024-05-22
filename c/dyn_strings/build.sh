if [ ! -d "target" ]; then
  echo "target dir does not exist. Creating..."
  mkdir target
fi

gcc -o ./target/main main.c dyn_strings.c -g -Wall -Wextra
