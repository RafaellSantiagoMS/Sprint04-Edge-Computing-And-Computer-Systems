# 🛰️ Sprint04 – Edge Computing and Computer Systems

## 👥 Integrantes
- **Enzo Augusto Lima da Silva** – RM: 562249  
- **Gustavo Neres dos Santos** – RM: 561785  
- **Pedro Marchese** – RM: 563339  
- **Rafaell Santiago** – RM: 563486  

---

## 🎯 Objetivo
Este projeto tem como objetivo implementar uma arquitetura IoT baseada em **Edge Computing** e **FIWARE**, utilizando o **Node-RED** como ferramenta de interface para visualização dos dados coletados por dispositivos IoT.  
A aplicação é executada em uma instância **EC2 da AWS**, com o intuito de demonstrar uma solução distribuída e replicável para monitoramento e controle de dispositivos conectados.

---

## ⚙️ Arquitetura da Solução

**Componentes principais:**
- **ESP32 (simulado no Wokwi):** coleta e envia dados dos sensores.  
- **Sensores simulados:**  
  - MPU6050 → Aceleração e Giroscópio  
- **Broker MQTT (Mosquitto):** responsável pela troca de mensagens entre o dispositivo e a instância FIWARE.  
- **Node-RED:** ferramenta de orquestração e dashboard para visualização e controle.  
- **AWS EC2 (Ubuntu Server):** hospeda o ambiente Node-RED e o FIWARE, garantindo a execução em nuvem.  

---

## ☁️ Criação da Instância EC2 e Instalação do Ambiente

### 1. Criar a Instância EC2
1. Acesse [AWS EC2](https://aws.amazon.com/ec2/) → **Launch Instance**  
2. Selecione **Ubuntu Server 24.04 LTS**  
3. Tipo de instância: `t2.micro` (Free Tier)  
4. Crie ou selecione um **Par de Chaves (.pem)** e baixe-o  
5. Configure o grupo de segurança com as portas abertas:  
   - 22 → SSH  
   - 1883 → MQTT Broker  
   - 1880 → Node-RED  
   - 1026 → Orion  
   - 4041 → IoT Agent MQTT  
   - 8666 → STH-Comet  
   - 27017 → MongoDB  

---

### 2. Acessar a Instância via SSH

Após criar e iniciar a instância, conecte-se a partir de qualquer computador que possua o par de chaves:

```bash
1. Primeiro ir até o sua maquina virtual no teminal de comando

- ssh -i nome-da-chave.pem ubuntu@<IP-PUBLICO-DA-EC2>

2. Instalar FIWARE
  - git clone https://github.com/fabiocabrini/fiware
  - sudo apt update
  - sudo apt install docker.io 
  - sudo apt install docker-compose
  - ls fiware
  - cd fiware
  - sudo docker-compose up -d

```
**Observação: O diretório fiware já contém o docker-compose configurado com os containers necessários e portas abertas.**

### 4. Instalar o Node-RED
```bash
1. Após ir até o sua maquina virtual (ssh -i nome-da-chave.pem ubuntu@<IP-PUBLICO-DA-EC2>)

Execute esses comandos : 
    - sudo npm install -g --unsafe-perm node-red
    - node-red

Depois é so acessar pelo navegadoer:

    - http://<IP_PUBLICO>:1880

```
## 🧠 Detalhes da Implementação

- O **ESP32** coleta dados de sensores (**MPU6050**) e envia via **MQTT** para o **Broker Mosquitto**.  
- O **Node-RED** consome os dados do broker e exibe no **Dashboard**, permitindo controle e visualização em tempo real.  
- O **FIWARE** processa os dados e armazena o histórico no **MongoDB**.  

---

## 🧾 Garantia de Replicabilidade

Para replicar a arquitetura:

1. Criar instância **EC2** com **Ubuntu Server**.  
2. Acessar via **SSH** com seu **par de chaves (.pem)**.  
3. Instalar **Docker**, **Docker Compose** e **Node-RED** conforme o guia acima.  
4. Clonar este repositório e iniciar os containers.  
5. Acessar o **Node-RED Dashboard** via `http://<IP_PUBLICO>:1880`.  

---

## 🔗 Simulação ESP32 (Wokwi)

[Link do Projeto Wokwi](https://wokwi.com/projects/445536592643160065)


## 📸 Prints

![Conexão do ESP32 com o IP da Instancia AWS](./src/images/Conexão%20do%20EPS32%20com%20o%20IP%20da%20instancia%20AWS%20.png)
![Conexão da Instacia AWS com o Node-Red](./src/images/Conexão%20e%20criação%20da%20Instancia%20AWS%20com%20o%20dashbord%20do%20Node-Red.png)
![Publicação de dados em tempo real do Mpu para os registros no gráfico](./src/images/Publicação%20de%20dados%20em%20tempo%20real%20do%20ESP32%20pro%20visualização%20no%20dashboard%20.png)

---




