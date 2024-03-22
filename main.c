#include<stdio.h>
#include<string.h>
#include<math.h>

struct appliance {
    char applianceName[20];
    int ratedPower;
    int quantity;
    int usageHours;
};

int n_appliances, dailyTotalWattage = 0, dailyTotalConsumption = 0;

//declaring variables that are part of the system configuration
float batteryEfficiency, chargeControllerEfficiency, wiringEfficiency, depthOfDischarge, powerFactor;
float voltageRatingModule, currentRatingModule, shortCircuitCurrent, safetyFactor, currentSystem, voltageSystem;
float numSeriesModule, numParallelModule, numTotalModule;
int sunHours, batteryVoltage, batteryCapacity, autonomyDays, currentController, voltageInverter;

// function prototypes
void inputAppliances(struct appliance appliances[]);
void displayAppliances(struct appliance appliances[]);
void inputSystemConfiguration();
void powerDemand();
void seriesParallel();
void batteryBank();
void chargeController();
void inverterSizing();

void main() {
    printf("Enter number of appliances: ");
    scanf("%d", &n_appliances);
    struct appliance appliances[n_appliances];
    inputAppliances(appliances);
    displayAppliances(appliances);
    inputSystemConfiguration();
    powerDemand();
    seriesParallel();
    batteryBank();
    chargeController();
    inverterSizing();
}

void inputAppliances(struct appliance appliances[]) {
    printf("\n");
    printf("Calculation of power demand \n");
    printf("\n");
    for (int i = 0; i < n_appliances; i++) {
        printf("Enter the name of the appliance: ");
        scanf("%s", appliances[i].applianceName);
        printf("Enter the power rating of the appliance: ");
        scanf("%d", &appliances[i].ratedPower);
        printf("Enter the quantity of the given appliance: ");
        scanf("%d", &appliances[i].quantity);
        printf("Enter the number of hours the appliance is used for: ");
        scanf("%d", &appliances[i].usageHours);
        printf("\n");
        printf("\n");
    }
}

void displayAppliances(struct appliance appliances[]) {
    printf("Appliance \t Rated Power \t Quantity \t Usage \t Total Wattage \t Total Consumption \n");
    for (int i = 0; i < n_appliances; i++) {
        int totalWattage = appliances[i].ratedPower * appliances[i].quantity;
        int totalConsumption = totalWattage * appliances[i].usageHours;
        printf("%-15s  %-15d  %-15d  %-15d  %-15d  %-15d", appliances[i].applianceName,
               appliances[i].ratedPower, appliances[i].quantity,
               appliances[i].usageHours, totalWattage, totalConsumption);
        printf("\n");
    }
    printf("\n");
    for (int i = 0; i < n_appliances; i++) {
        dailyTotalWattage += appliances[i].ratedPower * appliances[i].quantity;
        dailyTotalConsumption += appliances[i].ratedPower * appliances[i].quantity * appliances[i].usageHours;
    }
    printf("Overall Daily Total Wattage = %d Watts \n", dailyTotalWattage);
    printf("Overall Daily Total Consumption = %d Watt-hours per day \n", dailyTotalConsumption);
    printf("\n");
}

void inputSystemConfiguration() {
    printf("System Configuration \n");
    printf("\n");
    printf("Enter battery efficiency: ");
    scanf("%f", &batteryEfficiency);
    printf("Enter efficiency of charge controller: ");
    scanf("%f", &chargeControllerEfficiency);
    printf("Enter wiring efficiency: ");
    scanf("%f", &wiringEfficiency);
    printf("Enter depth of discharge of the battery: ");
    scanf("%f", &depthOfDischarge);
    printf("Enter the power factor: ");
    scanf("%f", &powerFactor);
    printf("Enter the voltage rating of the module: ");
    scanf("%f", &voltageRatingModule);
    printf("Enter the current rating of the module: ");
    scanf("%f", &currentRatingModule);
    printf("Enter the short circuit current: ");
    scanf("%f", &shortCircuitCurrent);
    printf("Enter the safety factor (should be greater than 1): ");
    scanf("%f", &safetyFactor);
    printf("Enter the DC voltage needed for the system: ");
    scanf("%f", &voltageSystem);

    printf("\n");

    printf("Enter average sun hours per day: ");
    scanf("%d", &sunHours);
    printf("Enter voltage provided by the battery: ");
    scanf("%d", &batteryVoltage);
    printf("Enter the capacity of one battery: ");
    scanf("%d", &batteryCapacity);
    printf("Enter the number of autonomy days: ");
    scanf("%d", &autonomyDays);
    printf("Enter the current in each controller: ");
    scanf("%d", &currentController);
    printf("Enter the voltage supplied by the inverter: ");
    scanf("%d", &voltageInverter);
    printf("\n");
}

void powerDemand() {
    printf("\n");
    printf("Sizing of solar array \n");
    printf("\n");
    float overallEfficieny = batteryEfficiency * chargeControllerEfficiency * wiringEfficiency;
    dailyTotalConsumption = ceil((float)dailyTotalConsumption / 100) * 100; //rounding up the consumption to the nearest hundred
    float totalPowerArray = dailyTotalConsumption/overallEfficieny;
    printf("Total power required from the array daily: %.2f Watt-hours per day \n", totalPowerArray);
    float peakPower = totalPowerArray/sunHours;
    printf("Peak power required from the array: %.2f Watts \n", peakPower);
    currentSystem = peakPower/voltageSystem;
    printf("Total current required for the system: %.2f Amperes \n", currentSystem);
}

void seriesParallel() {
    printf("\n");
    printf("Modules needed in series and parallel \n");
    printf("\n");
    numSeriesModule = ceil(voltageSystem/voltageRatingModule);
    numParallelModule = ceil(currentSystem/currentRatingModule);
    numTotalModule = numSeriesModule * numParallelModule;
    printf("Number of modules needed in series: %.1f \n", numSeriesModule);
    printf("Number of modules needed in parallel: %.1f \n", numParallelModule);
    printf("Number of modules needed overall: %.1f \n", numTotalModule);
}

void batteryBank() {
    printf("\n");
    printf("Sizing of the battery bank \n");
    printf("\n");
    float probablePowerNeeded = dailyTotalConsumption * autonomyDays;
    float bankCapacity = ceil(probablePowerNeeded/(depthOfDischarge * batteryEfficiency * batteryVoltage));
    float numBatteries = ceil(bankCapacity/batteryCapacity);
    float numBatteriesSeries = ceil(voltageSystem/batteryVoltage);
    float numBatteriesParallel = ceil(numBatteries/numBatteriesSeries);
    printf("Probable power storage needed: %.2f watt hours \n", probablePowerNeeded);
    printf("Battery bank capacity: %.2f ampere hours \n", bankCapacity);
    printf("Total number of batteries needed: %.1f \n", numBatteries);
    printf("Number of batteries required in series: %.1f \n", numBatteriesSeries);
    printf("Number of batteries require in parallel: %.1f \n", numBatteriesParallel);
}

void chargeController() {
    printf("\n");
    printf("Sizing of voltage or charge controller \n");
    printf("\n");
    float chargeControllerCapacity = shortCircuitCurrent * numParallelModule * safetyFactor;
    float numChargeController = ceil(chargeControllerCapacity/currentController);
    printf("Capacity of charge controller: %.2f Amperes \n", chargeControllerCapacity);
    printf("Number of controllers required: %.2f \n", numChargeController);
}

void inverterSizing() {
    printf("\n");
    printf("Sizing of the inverter \n");
    printf("\n");
    float loadCapacity = 1.25 * dailyTotalWattage;
    float currentInverter = loadCapacity/(voltageInverter * powerFactor);
    printf("Capacity of the inverter: %.2f watts \n", loadCapacity);
    printf("Current produced by the inverter: %.2f amperes \n", currentInverter);
}
