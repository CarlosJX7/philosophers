#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuración
PHILO_PATH="../philo"
TEST_DURATION=30
OUTPUT_FILE="output.txt"

# Función para imprimir encabezados
print_header() {
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}========================================${NC}"
}

# Función para verificar si existe el ejecutable
check_executable() {
    if [ ! -f "$PHILO_PATH" ]; then
        echo -e "${RED}Error: No se encuentra el ejecutable 'philo' en $PHILO_PATH${NC}"
        exit 1
    fi
    
    if [ ! -x "$PHILO_PATH" ]; then
        echo -e "${RED}Error: El archivo 'philo' no tiene permisos de ejecución${NC}"
        exit 1
    fi
}

# Test 1: 1 800 200 200 - El filósofo debe morir
test_1_should_die() {
    print_header "TEST 1: 1 800 200 200 (debe morir)"
    
    echo -e "${YELLOW}Ejecutando: $PHILO_PATH 1 800 200 200${NC}"
    echo -e "${YELLOW}Esperando que el filósofo muera...${NC}\n"
    
    # Ejecutar el programa y capturar output
    timeout 5s $PHILO_PATH 1 800 200 200 > $OUTPUT_FILE 2>&1
    
    # Verificar si apareció "died"
    if grep -q "died" $OUTPUT_FILE; then
        DEATH_TIME=$(grep "died" $OUTPUT_FILE | awk '{print $1}' | tr -d '[]')
        echo -e "${GREEN}✓ PASSED${NC}: El filósofo murió en ${DEATH_TIME}ms"
        
        # Verificar que murió cerca de 800ms
        if [ "$DEATH_TIME" -ge 790 ] && [ "$DEATH_TIME" -le 810 ]; then
            echo -e "${GREEN}✓ Tiempo de muerte correcto (~800ms)${NC}"
        else
            echo -e "${YELLOW}⚠ Advertencia: Murió en ${DEATH_TIME}ms (esperado ~800ms)${NC}"
        fi
        
        # Mostrar output
        echo -e "\n${BLUE}Output:${NC}"
        cat $OUTPUT_FILE
        
        return 0
    else
        echo -e "${RED}✗ FAILED${NC}: El filósofo NO murió"
        echo -e "\n${BLUE}Output:${NC}"
        cat $OUTPUT_FILE
        return 1
    fi
}

# Test 2: 5 800 200 200 - Ningún filósofo debe morir (30s)
test_2_no_death() {
    print_header "TEST 2: 5 800 200 200 (ninguno debe morir)"
    
    echo -e "${YELLOW}Ejecutando: $PHILO_PATH 5 800 200 200${NC}"
    echo -e "${YELLOW}Monitoreando durante ${TEST_DURATION} segundos...${NC}\n"
    
    # Ejecutar el programa en background
    timeout ${TEST_DURATION}s $PHILO_PATH 5 800 200 200 > $OUTPUT_FILE 2>&1 &
    PID=$!
    
    # Monitorear el proceso
    for i in $(seq 1 $TEST_DURATION); do
        if ! ps -p $PID > /dev/null 2>&1; then
            echo -e "${RED}✗ El proceso terminó prematuramente${NC}"
            break
        fi
        echo -ne "${YELLOW}Tiempo: ${i}/${TEST_DURATION}s\r${NC}"
        sleep 1
    done
    
    echo ""
    
    # Esperar a que termine
    wait $PID 2>/dev/null
    
    # Verificar si hubo muertes
    if grep -q "died" $OUTPUT_FILE; then
        DEATH_COUNT=$(grep -c "died" $OUTPUT_FILE)
        FIRST_DEATH=$(grep "died" $OUTPUT_FILE | head -1)
        echo -e "${RED}✗ FAILED${NC}: ${DEATH_COUNT} filósofo(s) murió/murieron"
        echo -e "${RED}Primera muerte: $FIRST_DEATH${NC}"
        
        # Mostrar últimas líneas del output
        echo -e "\n${BLUE}Últimas 20 líneas del output:${NC}"
        tail -20 $OUTPUT_FILE
        
        return 1
    else
        echo -e "${GREEN}✓ PASSED${NC}: Ningún filósofo murió durante ${TEST_DURATION} segundos"
        
        # Contar comidas
        EAT_COUNT=$(grep -c "is eating" $OUTPUT_FILE)
        echo -e "${GREEN}Total de comidas: $EAT_COUNT${NC}"
        
        # Mostrar últimas líneas del output
        echo -e "\n${BLUE}Últimas 10 líneas del output:${NC}"
        tail -10 $OUTPUT_FILE
        
        return 0
    fi
}

# Test 3: 5 800 200 200 7 - Deben comer 7 veces y parar
test_3_eat_and_stop() {
    print_header "TEST 3: 5 800 200 200 7 (deben comer 7 veces y parar)"
    
    echo -e "${YELLOW}Ejecutando: $PHILO_PATH 5 800 200 200 7${NC}"
    echo -e "${YELLOW}Esperando que todos coman 7 veces y la simulación pare...${NC}\n"
    
    # Ejecutar con timeout mayor
    timeout 60s $PHILO_PATH 5 800 200 200 7 > $OUTPUT_FILE 2>&1
    EXIT_CODE=$?
    
    # Verificar si hubo muertes
    if grep -q "died" $OUTPUT_FILE; then
        echo -e "${RED}✗ FAILED${NC}: Un filósofo murió"
        grep "died" $OUTPUT_FILE
        return 1
    fi
    
    # Contar comidas por filósofo
    echo -e "${BLUE}Contando comidas por filósofo:${NC}"
    PASSED=true
    for i in {1..5}; do
        COUNT=$(grep "^.* $i is eating" $OUTPUT_FILE | wc -l)
        if [ "$COUNT" -ge 7 ]; then
            echo -e "${GREEN}Filósofo $i: $COUNT comidas ✓${NC}"
        else
            echo -e "${RED}Filósofo $i: $COUNT comidas (esperado >= 7) ✗${NC}"
            PASSED=false
        fi
    done
    
    # Verificar que el programa terminó correctamente
    if [ $EXIT_CODE -eq 0 ]; then
        echo -e "${GREEN}✓ La simulación terminó correctamente${NC}"
    elif [ $EXIT_CODE -eq 124 ]; then
        echo -e "${YELLOW}⚠ Timeout: La simulación no terminó en 60s${NC}"
        PASSED=false
    fi
    
    if [ "$PASSED" = true ]; then
        echo -e "\n${GREEN}✓ PASSED${NC}: Todos comieron al menos 7 veces"
        return 0
    else
        echo -e "\n${RED}✗ FAILED${NC}"
        return 1
    fi
}

# Test 4: 4 410 200 200 - Ninguno debe morir
test_4_tight_timing() {
    print_header "TEST 4: 4 410 200 200 (ninguno debe morir)"
    
    echo -e "${YELLOW}Ejecutando: $PHILO_PATH 4 410 200 200${NC}"
    echo -e "${YELLOW}Timing ajustado - monitoreando 20 segundos...${NC}\n"
    
    timeout 20s $PHILO_PATH 4 410 200 200 > $OUTPUT_FILE 2>&1 &
    PID=$!
    
    for i in $(seq 1 20); do
        if ! ps -p $PID > /dev/null 2>&1; then
            break
        fi
        echo -ne "${YELLOW}Tiempo: ${i}/20s\r${NC}"
        sleep 1
    done
    
    echo ""
    wait $PID 2>/dev/null
    
    if grep -q "died" $OUTPUT_FILE; then
        echo -e "${RED}✗ FAILED${NC}: Un filósofo murió"
        grep "died" $OUTPUT_FILE
        return 1
    else
        echo -e "${GREEN}✓ PASSED${NC}: Ningún filósofo murió"
        EAT_COUNT=$(grep -c "is eating" $OUTPUT_FILE)
        echo -e "${GREEN}Total de comidas: $EAT_COUNT${NC}"
        return 0
    fi
}

# Test 5: 4 310 200 100 - Un filósofo debe morir
test_5_should_die() {
    print_header "TEST 5: 4 310 200 100 (debe morir)"
    
    echo -e "${YELLOW}Ejecutando: $PHILO_PATH 4 310 200 100${NC}"
    echo -e "${YELLOW}Timing imposible - esperando muerte...${NC}\n"
    
    timeout 5s $PHILO_PATH 4 310 200 100 > $OUTPUT_FILE 2>&1
    
    if grep -q "died" $OUTPUT_FILE; then
        DEATH_TIME=$(grep "died" $OUTPUT_FILE | head -1 | awk '{print $1}' | tr -d '[]')
        echo -e "${GREEN}✓ PASSED${NC}: Un filósofo murió en ${DEATH_TIME}ms"
        echo -e "\n${BLUE}Output:${NC}"
        tail -10 $OUTPUT_FILE
        return 0
    else
        echo -e "${RED}✗ FAILED${NC}: Ningún filósofo murió (debería morir)"
        return 1
    fi
}

# Función principal
main() {
    clear
    print_header "PHILOSOPHERS TESTER"
    echo -e "${BLUE}Tests basados en los requisitos obligatorios de 42${NC}\n"
    
    check_executable
    
    PASSED=0
    FAILED=0
    
    # Ejecutar tests
    if test_1_should_die; then ((PASSED++)); else ((FAILED++)); fi
    echo ""
    
    if test_2_no_death; then ((PASSED++)); else ((FAILED++)); fi
    echo ""
    
    if test_3_eat_and_stop; then ((PASSED++)); else ((FAILED++)); fi
    echo ""
    
    if test_4_tight_timing; then ((PASSED++)); else ((FAILED++)); fi
    echo ""
    
    if test_5_should_die; then ((PASSED++)); else ((FAILED++)); fi
    echo ""
    
    # Resumen final
    print_header "RESUMEN"
    echo -e "${GREEN}Tests pasados: $PASSED${NC}"
    echo -e "${RED}Tests fallados: $FAILED${NC}"
    echo -e "${BLUE}Total: $((PASSED + FAILED))${NC}\n"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}🎉 ¡Todos los tests pasaron!${NC}"
        exit 0
    else
        echo -e "${RED}❌ Algunos tests fallaron${NC}"
        exit 1
    fi
}

# Ejecutar
main