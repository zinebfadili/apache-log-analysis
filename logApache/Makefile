CC        = g++
CFLAGS    = -ansi -pedantic -Wall -Werror -Wextra -std=c++11
# ajout du flag -Wextra

EXEC_NAME = analog
INCLUDES  =
LIBS      =
SRCS      = $(wildcard *.cpp)
OBJS      = $(addprefix tmp/,$(notdir $(SRCS:.cpp=.o)))

ifdef DEBUG
	CFLAGS += -DMAP -g
endif


# On ne compile que les fichiers qui en ont besoin
normal: directories $(EXEC_NAME)

# On recompile tout
all: clean normal

# Cible de test
test: clean normal
	cd Tests/ && bash ./mktest.sh

# alias
tests: test

# Permet d'éviter que les fichiers du même nom que des règles
# soient "interprétés" comme des dépendances.
.PHONY: clean all normal test

# On supprime les fichiers temporaires et l'éxécutable
clean:
	rm -f $(EXEC_NAME) $(OBJS) 2>/dev/null || true
	rmdir -f tmp 2>/dev/null || true

# Création du répertoire temporaire
directories:
	mkdir -p tmp

# Règle de compilation
tmp/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

# Règle d'édition de liens
$(EXEC_NAME): $(OBJS)
	$(CC) -o $(EXEC_NAME) $(OBJS) $(LIBS)
