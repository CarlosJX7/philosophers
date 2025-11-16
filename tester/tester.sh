#!/bin/bash

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Variables
PHILO_PATH="./philo"
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Función para imprimir encabezado
print_header()
{
	echo "=================================="
	echo "   PHILOSOPHERS TESTER"
	echo "=================================="
	echo ""
}

# Función para verificar que el ejecutable existe
check_executable()
{
	if [ ! -f "$PHILO_PATH" ]; then
		echo -e "${RED}Error: No se encuentra el ejecutable $PHILO_PATH${NC}"
		exit 1
	fi
}

# Función para verificar muerte del filósofo
test_death_case()
{
	local test_name=$1
	local args=$2
	local expected_death_time=$3
	local tolerance=$4
	
	echo -e "${YELLOW}Test: $test_name${NC}"
	echo "Comando: $PHILO_PATH $args"
	
	TOTAL_TESTS=$((TOTAL_TESTS + 1))
	
	local output=$(timeout 5 $PHILO_PATH $args 2>&1)
	local exit_code=$?
	
	if [ $exit_code -eq 124 ]; then
		echo -e "${RED}✗ FAIL: El programa no terminó (timeout)${NC}"
		FAILED_TESTS=$((FAILED_TESTS + 1))
		echo ""
		return 1
	fi
	
	local death_line=$(echo "$output" | grep "died")
	
	if [ -z "$death_line" ]; then
		echo -e "${RED}✗ FAIL: No se detectó mensaje de muerte${NC}"
		FAILED_TESTS=$((FAILED_TESTS + 1))
		echo ""
		return 1
	fi
	
	local death_time=$(echo "$death_line" | grep -oE '^[0-9]+')
	local philo_num=$(echo "$death_line" | grep -oE '[0-9]+ died' | grep -oE '[0-9]+')
	
	echo "Output relevante:"
	echo "$output" | head -20
	echo "..."
	echo "$death_line"
	echo ""
	
	local time_diff=$((death_time - expected_death_time))
	if [ $time_diff -lt 0 ]; then
		time_diff=$((time_diff * -1))
	fi
	
	local passed=1
	
	if [ $time_diff -gt $tolerance ]; then
		echo -e "${RED}✗ FAIL: Tiempo de muerte incorrecto${NC}"
		echo "  Esperado: ~${expected_death_time}ms (±${tolerance}ms)"
		echo "  Obtenido: ${death_time}ms"
		echo "  Diferencia: ${time_diff}ms"
		passed=0
	else
		echo -e "${GREEN}✓ PASS: Tiempo de muerte correcto${NC}"
		echo "  Esperado: ~${expected_death_time}ms (±${tolerance}ms)"
		echo "  Obtenido: ${death_time}ms"
	fi
	
	local last_action=$(echo "$output" | grep -E "has taken a fork|is eating|is sleeping|is thinking" | tail -1)
	if [ -n "$last_action" ]; then
		local last_time=$(echo "$last_action" | grep -oE '^[0-9]+')
		local death_delay=$((death_time - last_time))
		
		if [ $death_delay -gt 10 ]; then
			echo -e "${RED}✗ FAIL: Retraso en detectar muerte > 10ms${NC}"
			echo "  Última acción: ${last_time}ms"
			echo "  Muerte reportada: ${death_time}ms"
			echo "  Retraso: ${death_delay}ms"
			passed=0
		else
			echo -e "${GREEN}✓ PASS: Retraso en detectar muerte ≤ 10ms${NC}"
		fi
	fi
	
	local message_broken=$(echo "$output" | grep -E "died.*died|fork.*eating" | grep -v "has taken a fork")
	if [ -n "$message_broken" ]; then
		echo -e "${RED}✗ FAIL: Mensajes rotos o mezclados detectados${NC}"
		passed=0
	else
		echo -e "${GREEN}✓ PASS: Formato de mensajes correcto${NC}"
	fi
	
	if [ $passed -eq 1 ]; then
		PASSED_TESTS=$((PASSED_TESTS + 1))
		echo -e "\n${GREEN}[RESULTADO: PASS]${NC}\n"
	else
		FAILED_TESTS=$((FAILED_TESTS + 1))
		echo -e "\n${RED}[RESULTADO: FAIL]${NC}\n"
	fi
	
	echo "=================================="
	echo ""
}

# Función para verificar que no hay data races con valgrind/helgrind
test_data_races()
{
	local args=$1
	
	echo -e "${YELLOW}Test: Verificación de data races${NC}"
	echo "Comando: valgrind --tool=helgrind $PHILO_PATH $args"
	
	TOTAL_TESTS=$((TOTAL_TESTS + 1))
	
	if ! command -v valgrind &> /dev/null; then
		echo -e "${YELLOW}⚠ SKIP: valgrind no está instalado${NC}"
		echo ""
		return 0
	fi
	
	local output=$(timeout 10 valgrind --tool=helgrind --log-fd=1 $PHILO_PATH $args 2>&1)
	
	local data_races=$(echo "$output" | grep -c "Possible data race")
	
	if [ $data_races -gt 0 ]; then
		echo -e "${RED}✗ FAIL: Se detectaron $data_races posibles data races${NC}"
		FAILED_TESTS=$((FAILED_TESTS + 1))
	else
		echo -e "${GREEN}✓ PASS: No se detectaron data races${NC}"
		PASSED_TESTS=$((PASSED_TESTS + 1))
	fi
	
	echo ""
}

# Función principal
main()
{
	print_header
	check_executable
	
	echo "Ejecutando tests de muerte de filósofos..."
	echo ""
	
	test_death_case "Un filósofo debe morir" "1 800 200 200" 800 10
	
	test_death_case "Un filósofo (muerte rápida)" "1 400 200 200" 400 10
	
	test_death_case "Dos filósofos (uno muere)" "2 400 200 100" 400 10
	
	test_death_case "Muerte con tiempo justo" "4 310 200 100" 310 10
	
	test_death_case "Cinco filósofos (uno muere)" "5 200 100 100" 200 10
	
	echo ""
	echo "=================================="
	echo "   RESUMEN DE RESULTADOS"
	echo "=================================="
	echo "Total de tests: $TOTAL_TESTS"
	echo -e "${GREEN}Tests pasados: $PASSED_TESTS${NC}"
	echo -e "${RED}Tests fallados: $FAILED_TESTS${NC}"
	echo "=================================="
	
	if [ $FAILED_TESTS -eq 0 ]; then
		echo -e "${GREEN}✓ Todos los tests pasaron correctamente${NC}"
		exit 0
	else
		echo -e "${RED}✗ Algunos tests fallaron${NC}"
		exit 1
	fi
}

main
