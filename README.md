# Monitoramento de Gás e Temperatura para Restaurantes

## 🧠 1. Descrição

Sistema para monitorar **temperatura** e **nível de gás** em cozinhas de restaurantes usando **ESP32**, sensores dedicados e alerta visual/sonoro. Esse projeto tem como objetivo auxiliar cozinheiros e bombeiros na preparação em cenários de incêndio ou intoxicação.

O dispositivo lê a temperatura via **DS18B20** e o nível de gás via **MQ2 (entrada analógica)**. Todos os dados são enviados via **MQTT** para um dashboard no **Node-RED**, permitindo acompanhamento em tempo real.

O sistema aciona automaticamente:

* **LEDs de status** (verde, amarelo, vermelho)
* **Buzzers de alarme**
* Indicadores no dashboard

É voltado para segurança preventiva: superaquecimento, vazamento de gás e condições perigosas.

📌 Protótipo desenvolvido na plataforma **Wokwi**:
👉 [Acesse aqui o projeto](https://wokwi.com/projects/448373635826729985)

---

## 🏗️ 2. Arquitetura da Solução

### 1. Sensoriamento

* **DS18B20** → temperatura da cozinha
* **Sensor de gás analógico** → leitura 0–4095 (ADC ESP32)
* LEDs e buzzers para alertas locais

### 2. Processamento (Edge)

* ESP32 faz:

  * Leitura dos sensores
  * Regras de alerta (thresholds)
  * Acionamento dos LEDs/buzzers
  * Publicação dos dados no MQTT

### 3. Comunicação

* Protocolo **MQTT**
* Broker: `test.mosquitto.org`

Tópicos:

```
topico_wokwi_temp
topico_wokwi_gas
```

### 4. Visualização

* Dashboard Node-RED com gauge para temperatura e gás
* Indicadores de alerta
* Históricos e logs de conexão

---

## 🔌 3. Diagramas e Visualização

### Wokwi

![Wokwi](src/assets/images/wokwi.jpg)

### Node-RED

![Node-RED](src/assets/images/node-red.jpg)

### Dashboard Node-RED

![Dashboard](src/assets/images/dashboard.jpg)

*(Gauges para gás e temperatura, baseados nos tópicos MQTT.)*

---

## ✅ 4. Funcionalidades

* **Monitoramento de temperatura**

  * Verde ← seguro
  * Amarelo ← acima de 30 °C (atenção)
  * Vermelho ← acima de 50 °C (alarme sonoro)

* **Monitoramento de gás**

  * Verde ← seguro
  * Amarelo ← entre 2750 e 3500 (atenção)
  * Vermelho ← acima de 3500 (alarme sonoro)

* **Alerta instantâneo**

  * LEDs dedicados (gás e temperatura)
  * Buzzers separados para cada risco

* **Comunicação IoT**

  * Publicação MQTT contínua (300 ms)
  * Indicadores de Wi-Fi e MQTT no próprio ESP32

---

## 🛠️ 5. Recursos Necessários

### Hardware

* ESP32 DevKit v4
* Sensor DS18B20
* Sensor MQ de gás (analógico)
* 6 LEDs (verde, amarelo, vermelho × 2)
* 2 buzzers
* Resistor para o DS18B20

### Software

* Arduino IDE ou Wokwi
* Node-RED + node-red-dashboard
* Broker MQTT `test.mosquitto.org`

---

## 📂 6. Estrutura do Projeto

```bash
├── sistema.ino             # Código ESP32
├── diagrama.json           # Esquemático Wokwi 
├── flows.json              # Fluxo do Node-RED
├── src
│   └── assets
│       └── images
│           └── dashboard.jpg 
│           └── node-red.jpg 
│           └── wokwi.jpg 
└── README.md
```

---

## ▶️ 7. Como Executar

1. Suba o código no ESP32 ou execute no Wokwi.
2. Abra o Node-RED e instale `node-red-dashboard`.
3. Configure MQTT para `test.mosquitto.org` porta 1883.
4. Importe o fluxo do dashboard.
5. Rode o ESP32: valores começam a aparecer nos gauges.

---

## 👥 Integrantes

* Gabriel Alexandre Fukushima Sakura
* Lucas Henrique Viana Estevam Sena

---

## 📜 Licença

Projeto acadêmico. Livre para uso educacional.

---