/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "st7735\st7735.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Comandos de Leitura dos botões
#define LER_BOTAO_A9		HAL_GPIO_ReadPin(BOTAO_A9_GPIO_Port, BOTAO_A9_Pin)
#define LER_BOTAO_A10		HAL_GPIO_ReadPin(BOTAO_A10_GPIO_Port, BOTAO_A10_Pin)
#define LER_BOTAO_A11		HAL_GPIO_ReadPin(BOTAO_A11_GPIO_Port, BOTAO_A11_Pin)
#define LER_BOTAO_A12		HAL_GPIO_ReadPin(BOTAO_A12_GPIO_Port, BOTAO_A12_Pin)
// Cores RGB565
#define ST7735_PRETO   		0x0000
#define ST7735_BRANCO   	0xFFFF
#define ST7735_VERMELHO     0xF800
#define ST7735_AZUL    		0x1F9D
#define ST7735_VERDE  		0x07D3
#define ST7735_VERDE2		0x0461
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
GPIO_PinState botaoA9, botaoA10, botaoA11, botaoA12;
int qtdeAlunos, tentativas, senha[4], entrada[4], matriculaLida;
int alunosPresentes = 0, totalSaidas = 0, alunosFora = 0, totalBloqueios = 0;
int presentesAnterior = -1, foraAnterior = -1;
char alunos[30], segundos[5]; // Textos
int maxFora = 3; // Limites de aluno fora de sala
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void menuBemVindo(void);
int etapa1(void);
void etapa2(void);
void etapa3(void);
void menuSenha(void);
void menuQtdeAlunos(void);
void menuAula(void);
void menuMatricula(void);
void atualizarAlunos(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(100);
	  menuBemVindo();

	  if (etapa1()) { // Só avança se a senha estiver correta
		  etapa2();
		  etapa3();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : BOTAO_A9_Pin BOTAO_A10_Pin BOTAO_A11_Pin BOTAO_A12_Pin */
  GPIO_InitStruct.Pin = BOTAO_A9_Pin|BOTAO_A10_Pin|BOTAO_A11_Pin|BOTAO_A12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
  void menuBemVindo(void) {
	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(8, 10, "Bem-vindo ao sistema", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(8, 23, "gerenciador de aulas", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(31, 45, "Pressione PA10", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(38, 58, "para iniciar", Font_7x10, ST7735_VERDE, ST7735_PRETO);
  }

  int etapa1(void) {
	  botaoA10 = LER_BOTAO_A10;
	  while (botaoA10 != GPIO_PIN_RESET) {
		  botaoA10 = LER_BOTAO_A10;
	  }
	  HAL_Delay(100);
	  while (botaoA10 == GPIO_PIN_RESET) {
		  botaoA10 = LER_BOTAO_A10;
	  }

	  uint32_t seed = HAL_GetTick(); // Gerar seed para senha aleatória
	  srand(seed);

	  for (int i = 0; i < 4; i++) {
		  senha[i] = (rand() % 4) + 1; // Senha aleatória de 4 dígitos
		  entrada[i] = 0;
	  }

	  tentativas = 0;

	  while (1) {
		  int indice = 0;
		  HAL_Delay(100);
		  menuSenha();

		  while (indice < 4) {
			  botaoA9 = LER_BOTAO_A9;
			  botaoA10 = LER_BOTAO_A10;
			  botaoA11 = LER_BOTAO_A11;
			  botaoA12 = LER_BOTAO_A12;

			  if (botaoA9 == GPIO_PIN_RESET) {
				  HAL_Delay(100);
				  while (botaoA9 == GPIO_PIN_RESET) {
					  botaoA9 = LER_BOTAO_A9;
				  }
				  entrada[indice++] = 1;
				  switch (indice) {
				  case 1:
					  ST7735_WriteString(50, 63, "1", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 2:
					  ST7735_WriteString(59, 63, "1", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 3:
					  ST7735_WriteString(68, 63, "1", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 4:
					  ST7735_WriteString(77, 63, "1", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  }
			  }
			  else if (botaoA10 == GPIO_PIN_RESET) {
				  HAL_Delay(100);
				  while (botaoA10 == GPIO_PIN_RESET) {
					  botaoA10 = LER_BOTAO_A10;
				  }
				  entrada[indice++] = 2;
				  switch (indice) {
				  case 1:
					  ST7735_WriteString(50, 63, "2", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 2:
					  ST7735_WriteString(59, 63, "2", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
			  	  case 3:
			  		  ST7735_WriteString(68, 63, "2", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
			  	  case 4:
			  		  ST7735_WriteString(77, 63, "2", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  }
			  }
			  else if (botaoA11 == GPIO_PIN_RESET) {
				  HAL_Delay(100);
				  while (botaoA11 == GPIO_PIN_RESET) {
					  botaoA11 = LER_BOTAO_A11;
				  }
				  entrada[indice++] = 3;
				  switch (indice) {
				  case 1:
					  ST7735_WriteString(50, 63, "3", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 2:
					  ST7735_WriteString(59, 63, "3", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 3:
					  ST7735_WriteString(68, 63, "3", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  case 4:
					  ST7735_WriteString(77, 63, "3", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  }
			  }
			  else if (botaoA12 == GPIO_PIN_RESET) {
				  HAL_Delay(100);
				  while (botaoA12 == GPIO_PIN_RESET) {
					  botaoA12 = LER_BOTAO_A12;
				  }
				  entrada[indice++] = 4;
				  switch (indice) {
				  case 1:
					  ST7735_WriteString(50, 63, "4", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
			  	  case 2:
			  		  ST7735_WriteString(59, 63, "4", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
			  	  case 3:
			  		  ST7735_WriteString(68, 63, "4", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
			  	  case 4:
			  		  ST7735_WriteString(77, 63, "4", Font_7x10, ST7735_BRANCO, ST7735_PRETO); break;
				  }
			  }
		  }

		  _Bool correto = 1;
		  for (int i = 0; i < 4; i++) { // Conferência da senha dígito por dígito
			  if (entrada[i] != senha[i]) {
				  correto = 0;
				  break;
			  }
		  }

		  if (correto) {
			  ST7735_FillScreen(ST7735_PRETO);
			  ST7735_WriteString(25, 35, "Acesso liberado", Font_7x10, ST7735_VERDE, ST7735_PRETO);
			  HAL_Delay(2000);
			  return 1;
		  }
		  else {
			  tentativas++;
			  if (tentativas >= 3) {
				  totalBloqueios++;
				  if (totalBloqueios >= 2) {
					  ST7735_FillScreen(ST7735_PRETO);
					  ST7735_WriteString(18, 25, "Sistema bloqueado", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
					  ST7735_WriteString(13, 43, "TEMPO INDETERMINADO", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
					  while (1) { // Bloqueio permanente - Reiniciar placa
					  }
				  }
				  ST7735_FillScreen(ST7735_PRETO);
				  ST7735_WriteString(18, 20, "Sistema bloqueado", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  ST7735_WriteString(21, 38, "Voltando ao menu", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);

				  for (int i = 15; i > 0; i--) { // Bloqueio de 15 segundos
					  sprintf(segundos, "%d", i);
					  ST7735_WriteString(75, 56, segundos, Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
					  HAL_Delay(1000);
					  ST7735_WriteString(75, 56, "   ", Font_7x10, ST7735_PRETO, ST7735_PRETO);
				  }
				  tentativas = 0;
				  return 0; // Volta ao menu de bem-vindo
			  }
			  else {
				  ST7735_FillScreen(ST7735_PRETO);
				  ST7735_WriteString(25, 25, "Senha incorreta", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  ST7735_WriteString(25, 43, "Tente novamente", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  HAL_Delay(3000);
			  }
		  }
	  }
  }

  void etapa2(void) {
	  qtdeAlunos = 0;
	  HAL_Delay(100);
	  menuQtdeAlunos();

	  while (1) {
		  botaoA9 = LER_BOTAO_A9;
		  botaoA10 = LER_BOTAO_A10;
		  botaoA11 = LER_BOTAO_A11;
		  botaoA12 = LER_BOTAO_A12;

		  if (botaoA9 == GPIO_PIN_RESET) {
			  HAL_Delay(100);
			  while (botaoA9 == GPIO_PIN_RESET) {
				  botaoA9 = LER_BOTAO_A9;
			  }
			  qtdeAlunos += 5;
			  sprintf(alunos, "%d ", qtdeAlunos);
			  ST7735_WriteString(50, 65, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);
		  }
		  else if (botaoA10 == GPIO_PIN_RESET) {
			  HAL_Delay(100);
			  while (botaoA10 == GPIO_PIN_RESET) {
				  botaoA10 = LER_BOTAO_A10;
			  }
			  qtdeAlunos += 1;
			  sprintf(alunos, "%d ", qtdeAlunos);
			  ST7735_WriteString(50, 65, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);
		  }
		  else if (botaoA11 == GPIO_PIN_RESET) {
			  HAL_Delay(100);
			  while (botaoA11 == GPIO_PIN_RESET) {
				  botaoA11 = LER_BOTAO_A11;
			  }
			  if (qtdeAlunos > 0) {
				  qtdeAlunos -= 1;
				  sprintf(alunos, "%d ", qtdeAlunos);
				  ST7735_WriteString(50, 65, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);
			  }
			  else {
				  ST7735_FillScreen(ST7735_PRETO);
				  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  ST7735_WriteString(13, 43, "Quantidade negativa", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  HAL_Delay(2000);
				  menuQtdeAlunos();
			  }
		  }
		  else if (botaoA12 == GPIO_PIN_RESET) {
			  HAL_Delay(100);
			  while (botaoA12 == GPIO_PIN_RESET) {
				  botaoA12 = LER_BOTAO_A12;
			  }
			  if (qtdeAlunos > 0) { // Mínimo de 1 aluno na turma
				  sprintf(alunos, "com %d alunos", qtdeAlunos);
				  ST7735_FillScreen(ST7735_PRETO);
				  ST7735_WriteString(24, 25, "Turma confirmada", Font_7x10, ST7735_VERDE, ST7735_PRETO);
				  ST7735_WriteString(35, 43, alunos, Font_7x10, ST7735_VERDE, ST7735_PRETO);
				  HAL_Delay(2000);
				  break;
			  }
			  else {
				  ST7735_FillScreen(ST7735_PRETO);
				  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  ST7735_WriteString(31, 43, "Minimo 1 aluno", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
				  HAL_Delay(2000);
				  menuQtdeAlunos();
			  }
		  }
	  }
  }

  void etapa3() {
	  alunosPresentes = 0;
	  totalSaidas = 0;
	  alunosFora = 0;
	  _Bool aulaEmCurso = 1;
	  uint32_t inicioAula = HAL_GetTick(); // Tempo de início da aula

	  HAL_Delay(100);
	  menuAula();

	  while (aulaEmCurso) { // Só atualiza a barra se os valores tiverem sido alterados
		  if (alunosPresentes != presentesAnterior || alunosFora != foraAnterior) {
			  atualizarAlunos();
		  }

		  botaoA9 = LER_BOTAO_A9;
		  botaoA10 = LER_BOTAO_A10;
		  botaoA11 = LER_BOTAO_A11;
		  botaoA12 = LER_BOTAO_A12;

		  if (botaoA9 == GPIO_PIN_RESET) { // Entrada de aluno
			  HAL_Delay(100);
			  while (botaoA9 == GPIO_PIN_RESET) {
				  botaoA9 = LER_BOTAO_A9;
	  		  }
	  		  if (alunosPresentes < qtdeAlunos) { // Não pode exceder o limite
	  			menuMatricula();
	  			matriculaLida = 99999;

	  			// Simulação de leitor de matrícula - Espera variável no Debug
	  			while (matriculaLida == 99999) {
	  				botaoA11 = LER_BOTAO_A11;
	  				botaoA12 = LER_BOTAO_A12;
	  				if (botaoA11 == GPIO_PIN_RESET && botaoA12 == GPIO_PIN_RESET) {
	  					HAL_Delay(100);
	  					botaoA11 = LER_BOTAO_A11;
	  					botaoA12 = LER_BOTAO_A12;
	  					if (botaoA11 == GPIO_PIN_RESET && botaoA12 == GPIO_PIN_RESET) {
	  						while (botaoA11 == GPIO_PIN_RESET || botaoA12 == GPIO_PIN_RESET) {
	  							botaoA11 = LER_BOTAO_A11;
	  							botaoA12 = LER_BOTAO_A12;
	  						}
	  						matriculaLida = 9999; // Valor padrão caso não digite nada
	  					}
	  				}
	  			}

	  			if (matriculaLida == 9999) {
	  				alunosPresentes++;
	  				ST7735_FillScreen(ST7735_PRETO);
	  				ST7735_WriteString(17, 38, "Entrada confirmada", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  				HAL_Delay(2000);
	  			}
	  			else if (matriculaLida == 0) {
	  				ST7735_FillScreen(ST7735_PRETO);
	  				ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  				ST7735_WriteString(17, 43, "Matricula invalida", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  				HAL_Delay(2000);
	  			}
	  			else {
	  				alunosPresentes++;
	  				ST7735_FillScreen(ST7735_PRETO);
	  				ST7735_WriteString(17, 25, "Entrada confirmada", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  				sprintf(alunos, "Matricula: %d", matriculaLida);
	  				ST7735_WriteString(27, 43, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  				HAL_Delay(2000);
	  			}
	  			matriculaLida = 0;
	  			HAL_Delay(100);
	  			menuAula();
	  		  }
	  		  else {
	  			  ST7735_FillScreen(ST7735_PRETO);
	  			  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  		      ST7735_WriteString(38, 43, "Turma lotada", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  			  HAL_Delay(2000);
	  			  menuAula();
	  		  }
	  	  }
		  else if (botaoA10 == GPIO_PIN_RESET) { // Saída de aluno
	  		  HAL_Delay(100);
	  		  while (botaoA10 == GPIO_PIN_RESET) {
	  			botaoA10 = LER_BOTAO_A10;
	  		  }
	  		  if (alunosFora >= maxFora) { // Não sair mais do que o permitido
	  			  ST7735_FillScreen(ST7735_PRETO);
	  			  sprintf(alunos, "Maximo %d fora", maxFora);
	  			  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  			  ST7735_WriteString(34, 43, alunos, Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  		  }
	  		  else if (alunosPresentes <= alunosFora) {
	  			  ST7735_FillScreen(ST7735_PRETO);
	  			  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  			  ST7735_WriteString(45, 43, "Sala vazia", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  		  }
	  		  else {
	  			  ST7735_FillScreen(ST7735_PRETO);
	  			  ST7735_WriteString(31, 38, "Saida liberada", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  			  alunosFora++;
	  			  totalSaidas++;
	  		  }
	  		  HAL_Delay(2000);
	  		  menuAula();
	  	  }
		  // PA11+PA12 encerra aula - PA11 retorna aluno
		  else if (botaoA11 == GPIO_PIN_RESET || botaoA12 == GPIO_PIN_RESET) {
			  HAL_Delay(100);
			  botaoA11 = LER_BOTAO_A11;
			  botaoA12 = LER_BOTAO_A12;
	  		  if (botaoA11 == GPIO_PIN_RESET && botaoA12 == GPIO_PIN_RESET) {
	  			  botaoA11 = LER_BOTAO_A11;
	  			  botaoA12 = LER_BOTAO_A12;
	  			  while (botaoA11 == GPIO_PIN_RESET || botaoA12 == GPIO_PIN_RESET) {
	  				botaoA11 = LER_BOTAO_A11;
	  				botaoA12 = LER_BOTAO_A12;
	  			  }
	  			  if (alunosFora > 0) { // Impede encerrar a aula com alunos para fora
	  				  ST7735_FillScreen(ST7735_PRETO);
	  				  ST7735_WriteString(20, 25, "Nao pode encerrar", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  				  ST7735_WriteString(27, 43, "com alunos fora", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  				  HAL_Delay(2000);
	  				  menuAula();
	  			  }
	  		      else {
	  				  aulaEmCurso = 0; // Quebra o while de execução da aula
	  			  }
	  		  }
	  		  else if (botaoA11 == GPIO_PIN_RESET) {
	  			  while (botaoA11 == GPIO_PIN_RESET) {
	  				botaoA11 = LER_BOTAO_A11;
	  			  }
	  			  if (alunosFora > 0) {
	  				  ST7735_FillScreen(ST7735_PRETO);
	  				  ST7735_WriteString(24, 38, "Volta registrada", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  				  alunosFora--;
	  			  }
	  			  else {
	  				  ST7735_FillScreen(ST7735_PRETO);
	  				  ST7735_WriteString(62, 25, "Erro:", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  				  ST7735_WriteString(38, 43, "Ninguem fora", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  			  }
	  			  HAL_Delay(2000);
	  			  menuAula();
	  		  }
	  		  else if (botaoA12 == GPIO_PIN_RESET) {
	  			  while (botaoA12 == GPIO_PIN_RESET) {
	  				botaoA12 = LER_BOTAO_A12;
	  			  }
	  		  }
	  	  }
	  }

	  uint32_t fimAula = HAL_GetTick(); // Tempo de fim da aula
	  uint32_t tempoAula = (fimAula - inicioAula) / 1000; // Cálculo do tempo em segundos
	  int minutos = tempoAula / 60;
	  int segs = tempoAula % 60;

	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(31, 10, "AULA ENCERRADA", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);

	  ST7735_WriteString(5, 25, "Total Registrados:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  sprintf(alunos, "%d", alunosPresentes);
	  ST7735_WriteString(138, 25, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  ST7735_WriteString(5, 37, "Total de Saidas:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  sprintf(alunos, "%d", totalSaidas);
	  ST7735_WriteString(124, 37, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  ST7735_WriteString(5, 49, "Tempo de Aula:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  sprintf(alunos, "%d:%02d", minutos, segs);
	  ST7735_WriteString(110, 49, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  ST7735_WriteString(5, 65, "Reiniciando...", Font_7x10, ST7735_VERMELHO, ST7735_PRETO);
	  HAL_Delay(20000);
  }

  void menuSenha() {
	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(5, 5, "Digite a senha:", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(5, 30, "PA9:  ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(42, 30, "1", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 42, "PA10: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(42, 42, "2", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(80, 30, "PA11: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(117, 30, "3", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(80, 42, "PA12: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(117, 42, "4", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 65, "Senha:", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(50, 65, "_", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(59, 65, "_", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(68, 65, "_", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(77, 65, "_", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
  }

  void menuQtdeAlunos() {
	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(5, 5, "Quantidade de alunos", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(5, 30, "PA9:  ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(42, 30, "+5", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 42, "PA10: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(42, 42, "+1", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(80, 30, "PA11: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(117, 30, "-1", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(80, 42, "PA12: ", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(117, 42, "OK", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 65, "Total:    alunos", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  sprintf(alunos, "%d ", qtdeAlunos);
	  ST7735_WriteString(50, 65, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);
  }

  void menuAula() {
	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(5, 5, "Presentes:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(5, 17, "Fora de sala:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(5, 29, "PA9:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(40, 29, "Chegada", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 41, "PA10:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(47, 41, "Saida", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 53, "PA11:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(47, 53, "Volta", Font_7x10, ST7735_BRANCO, ST7735_PRETO);
	  ST7735_WriteString(5, 65, "PA11+PA12:", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(82, 65, "Fim", Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  presentesAnterior = -1; // Reseta os estados de controle
	  foraAnterior = -1;
  }

  void menuMatricula() {
	  ST7735_FillScreen(ST7735_PRETO);
	  ST7735_WriteString(17, 15, "Cadastro Matricula", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(10, 27, "Insira ID pelo Debug", Font_7x10, ST7735_VERDE, ST7735_PRETO);
	  ST7735_WriteString(38, 45, "ou pressione", Font_7x10, ST7735_AZUL, ST7735_PRETO);
	  ST7735_WriteString(10, 57, "PA11+PA12 para pular", Font_7x10, ST7735_AZUL, ST7735_PRETO);
  }

  void atualizarAlunos() {
	  sprintf(alunos, "%d / %d ", alunosPresentes, qtdeAlunos);
	  ST7735_WriteString(82, 5, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  // Barra vertical de preenchimento da sala
	  int alturaVerde = (alunosPresentes * (70 - 2)) / qtdeAlunos;
	  ST7735_FillRectangle(148, 5, 7, 70, ST7735_BRANCO); // Borda

	  if (alturaVerde < (70 - 2)) {
		  int alturaPreta = (70 - 2) - alturaVerde;
		  ST7735_FillRectangle(148 + 1, 5 + 1, 7 - 2, alturaPreta, ST7735_PRETO); // Fundo preto
	  }

	  if (alturaVerde > 0) { // Progresso
	  	  int yVerde = 5 + 70 - 1 - alturaVerde;
	  	  ST7735_FillRectangle(148 + 1, yVerde, 7 - 2, alturaVerde, ST7735_VERDE2);
	  }

	  sprintf(alunos, "%d / %d ", alunosFora, maxFora);
	  ST7735_WriteString(103, 17, alunos, Font_7x10, ST7735_BRANCO, ST7735_PRETO);

	  presentesAnterior = alunosPresentes; // Salva estado atual
	  foraAnterior = alunosFora;
  }
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
