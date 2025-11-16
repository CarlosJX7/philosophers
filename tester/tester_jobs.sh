#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuración
PHILO_PATH="../philo"
OUTPUT_DIR="./test_outputs"
LOCK_FILE="/tmp/philo_tester.lock"

# Crear directorio para outputs
mkdir -p "$OUTPUT_DIR"

# Función para imprimir con color y lock (thread-safe)
print_safe() {
    local color=$1
    local message=$2
    (
        flock -x 200
        echo -e "${color}${message}${NC}"
    ) 200>"$LOCK_FILE"
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
    local output_file="$OUTPUT_DIR/test1.txt"
    local test_name="TEST 1: 1 800 200 200"
    
    print_safe "$CYAN" "[$test_name] Iniciando..."
    
    timeout 5s $PHILO_PATH 1 800 200 200 > "$output_file" 2>&1
    
    if grep -q "died" "$output_file"; then
        DEATH_TIME=$(grep "died" "$output_file" | awk '{print $1}' | tr -d '[]')
        
        if [ "$DEATH_TIME" -ge 790 ] && [ "$DEATH_TIME" -le 810 ]; then
            print_safe "$GREEN" "[$test_name] ✓ PASSED - Murió en ${DEATH_TIME}ms"
            echo "PASSED" > "$OUTPUT_DIR/test1.result"
        else
            print_safe "$YELLOW" "[$test_name] ⚠ PASSED con advertencia - Murió en ${DEATH_TIME}ms (esperado ~800ms)"
            echo "PASSED" > "$OUTPUT_DIR/test1.result"
        fi
    else
        print_safe "$RED" "[$test_name] ✗ FAILED - El filósofo NO murió"
        echo "FAILED" > "$OUTPUT_DIR/test1.result"
    fi
}

# Test 2: 5 800 200 200 - Ningún filósofo debe morir (30s)
test_2_no_death() {
    local output_file="$OUTPUT_DIR/test2.txt"
    local test_name="TEST 2: 5 800 200 200"
    local duration=30
    
    print_safe "$CYAN" "[$test_name] Iniciando (${duration}s)..."
    
    timeout ${duration}s $PHILO_PATH 5 800 200 200 > "$output_file" 2>&1
    
    if grep -q "died" "$output_file"; then
        DEATH_COUNT=$(grep -c "died" "$output_file")
        print_safe "$RED" "[$test_name] ✗ FAILED - ${DEATH_COUNT} filósofo(s) murió/murieron"
        echo "FAILED" > "$OUTPUT_DIR/test2.result"
    else
        EAT_COUNT=$(grep -c "is eating" "$output_file")
        print_safe "$GREEN" "[$test_name] ✓ PASSED - Sin muertes (${EAT_COUNT} comidas)"
        echo "PASSED" > "$OUTPUT_DIR/test2.result"
    fi
}

# Test 3: 5 800 200 200 7 - Deben comer 7 veces y parar
test_3_eat_and_stop() {
    local output_file="$OUTPUT_DIR/test3.txt"
    local test_name="TEST 3: 5 800 200 200 7"
    
    print_safe "$CYAN" "[$test_name] Iniciando..."
    
    timeout 60s $PHILO_PATH 5 800 200 200 7 > "$output_file" 2>&1
    EXIT_CODE=$?
    
    if grep -q "died" "$output_file"; then
        print_safe "$RED" "[$test_name] ✗ FAILED - Un filósofo murió"
        echo "FAILED" > "$OUTPUT_DIR/test3.result"
        return
    fi
    
    local all_ate=true
    local meal_counts=""
    for i in {1..5}; do
        COUNT=$(grep "^.* $i is eating" "$output_file" | wc -l)
        meal_counts="$meal_counts P$i:$COUNT"
        if [ "$COUNT" -lt 7 ]; then
            all_ate=false
        fi
    done
    
    if [ "$all_ate" = true ] && [ $EXIT_CODE -eq 0 ]; then
        print_safe "$GREEN" "[$test_name] ✓ PASSED - Todos comieron >=7 veces ($meal_counts)"
        echo "PASSED" > "$OUTPUT_DIR/test3.result"
    else
        if [ $EXIT_CODE -eq 124 ]; then
            print_safe "$RED" "[$test_name] ✗ FAILED - Timeout ($meal_counts)"
        else
            print_safe "$RED" "[$test_name] ✗ FAILED - No comieron suficiente ($meal_counts)"
        fi
        echo "FAILED" > "$OUTPUT_DIR/test3.result"
    fi
}

# Test 4: 4 410 200 200 - Ninguno debe morir
test_4_tight_timing() {
    local output_file="$OUTPUT_DIR/test4.txt"
    local test_name="TEST 4: 4 410 200 200"
    local duration=20
    
    print_safe "$CYAN" "[$test_name] Iniciando (${duration}s)..."
    
    timeout ${duration}s $PHILO_PATH 4 410 200 200 > "$output_file" 2>&1
    
    if grep -q "died" "$output_file"; then
        print_safe "$RED" "[$test_name] ✗ FAILED - Un filósofo murió"
        echo "FAILED" > "$OUTPUT_DIR/test4.result"
    else
        EAT_COUNT=$(grep -c "is eating" "$output_file")
        print_safe "$GREEN" "[$test_name] ✓ PASSED - Sin muertes (${EAT_COUNT} comidas)"
        echo "PASSED" > "$OUTPUT_DIR/test4.result"
    fi
}

# Test 5: 4 310 200 100 - Un filósofo debe morir
test_5_should_die() {
    local output_file="$OUTPUT_DIR/test5.txt"
    local test_name="TEST 5: 4 310 200 100"
    
    print_safe "$CYAN" "[$test_name] Iniciando..."
    
    timeout 5s $PHILO_PATH 4 310 200 100 > "$output_file" 2>&1
    
    if grep -q "died" "$output_file"; then
        DEATH_TIME=$(grep "died" "$output_file" | head -1 | awk '{print $1}' | tr -d '[]')
        print_safe "$GREEN" "[$test_name] ✓ PASSED - Murió en ${DEATH_TIME}ms"
        echo "PASSED" > "$OUTPUT_DIR/test5.result"
    else
        print_safe "$RED" "[$test_name] ✗ FAILED - Ningún filósofo murió (debería morir)"
        echo "FAILED" > "$OUTPUT_DIR/test5.result"
    fi
}

# Barra de progreso
show_progress() {
    local total=$1
    local current=0
    
    while [ $current -lt $total ]; do
        current=0
        for i in {1..5}; do
            if [ -f "$OUTPUT_DIR/test${i}.result" ]; then
                ((current++))
            fi
        done
        
        local percent=$((current * 100 / total))
        local filled=$((current * 50 / total))
        local empty=$((50 - filled))
        
        printf "\r${BLUE}Progreso: ["
        printf "%${filled}s" | tr ' ' '█'
        printf "%${empty}s" | tr ' ' '░'
        printf "] %d/%d tests (${percent}%%)${NC}" $current $total
        
        if [ $current -lt $total ]; then
            sleep 0.5
        fi
    done
    echo ""
}

# Función principal
main() {
    clear
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}   PHILOSOPHERS PARALLEL TESTER${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "${BLUE}Ejecutando todos los tests en paralelo...${NC}\n"
    
    check_executable
    
    # Limpiar resultados anteriores
    rm -f "$OUTPUT_DIR"/*.result
    rm -f "$LOCK_FILE"
    
    # Ejecutar todos los tests en paralelo
    test_1_should_die &
    test_2_no_death &
    test_3_eat_and_stop &
    test_4_tight_timing &
    test_5_should_die &
    
    # Mostrar barra de progreso
    show_progress 5
    
    # Esperar a que todos terminen
    wait
    
    echo ""
    
    # Contar resultados
    PASSED=0
    FAILED=0
    
    for i in {1..5}; do
        if [ -f "$OUTPUT_DIR/test${i}.result" ]; then
            result=$(cat "$OUTPUT_DIR/test${i}.result")
            if [ "$result" = "PASSED" ]; then
                ((PASSED++))
            else
                ((FAILED++))
            fi
        fi
    done
    
    # Resumen final
    echo -e "\n${BLUE}========================================${NC}"
    echo -e "${BLUE}              RESUMEN${NC}"
    echo -e "${BLUE}========================================${NC}"
    echo -e "${GREEN}✓ Tests pasados: $PASSED${NC}"
    echo -e "${RED}✗ Tests fallados: $FAILED${NC}"
    echo -e "${BLUE}Total: $((PASSED + FAILED))${NC}\n"
    
    # Mostrar detalles de tests fallidos
    if [ $FAILED -gt 0 ]; then
        echo -e "${YELLOW}Detalles de tests fallidos:${NC}"
        for i in {1..5}; do
            if [ -f "$OUTPUT_DIR/test${i}.result" ]; then
                result=$(cat "$OUTPUT_DIR/test${i}.result")
                if [ "$result" = "FAILED" ]; then
                    echo -e "\n${RED}═══ Test $i - Output (últimas 15 líneas) ═══${NC}"
                    tail -15 "$OUTPUT_DIR/test${i}.txt"
                fi
            fi
        done
    fi
    
    echo -e "\n${CYAN}Outputs completos guardados en: $OUTPUT_DIR/${NC}\n"
    
    # Limpiar lock file
    rm -f "$LOCK_FILE"
    
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