\c unmud;

CREATE DOMAIN UBIGINT AS BIGINT NOT NULL CHECK(VALUE >= 0) DEFAULT 0;

CREATE TYPE DIRECTIONS AS ENUM ('NORTH', 'EAST', 'SOUTH', 'WEST');
CREATE TYPE MAPTYPE AS ENUM ('PLAINROOM', 'TRAININGROOM', 'STORE');
CREATE TYPE ITEMTYPE AS ENUM ('HEALING', 'WEAPON', 'ARMOR');
CREATE TYPE PLAYERRANK AS ENUM ('REGULAR', 'GOD', 'ADMIN');

CREATE TYPE ATTRIBUTE AS (
    strength INTEGER,
    health INTEGER,
    agility INTEGER,
    maxHitPoints INTEGER,
    accuracy INTEGER,
    dodging INTEGER,
    strikeDamage INTEGER,
    damageAbsorb INTEGER,
    hpRegen INTEGER
);


CREATE TABLE Item(
    id BIGSERIAL,
    name TEXT,
    type ITEMTYPE,
    min INTEGER,
    max INTEGER,
    speed INTEGER,
    price UBIGINT,
    attributes ATTRIBUTE,

    CONSTRAINT Item_PK PRIMARY KEY(id)
);


CREATE TABLE Store(
    id BIGSERIAL,
    name TEXT,

    CONSTRAINT Store_PK PRIMARY KEY(id)
);

CREATE TABLE StoreVendeItem(
    storeId BIGINT,
    itemId BIGINT,

    CONSTRAINT StoreVendeItem_Item_FK FOREIGN KEY(itemId) REFERENCES Item(id),
    CONSTRAINT StoreVendeItem_Store_FK FOREIGN KEY(storeId) REFERENCES Store(id)
);


CREATE TABLE Enemy(
    id BIGSERIAL,
    name TEXT,
    hitPoints INTEGER,
    accuracy INTEGER,
    dodging INTEGER,
    strikeDamage INTEGER,
    damageAbsorb INTEGER,
    experience INTEGER,
    weaponId BIGINT,
    moneyMin UBIGINT,
    moneyMax UBIGINT,

    CONSTRAINT Enemy_PK PRIMARY KEY(id),
    CONSTRAINT Enemy_Item_FK FOREIGN KEY(weaponId) REFERENCES Item(id)
);

CREATE TABLE Loot(
    enemyId BIGINT,
    itemId BIGINT,
    itemQuantity INTEGER,

    CONSTRAINT Loot_Enemy_FK FOREIGN KEY(enemyId) REFERENCES Enemy(id),
    CONSTRAINT Loot_Item_FK FOREIGN KEY(itemId) REFERENCES Item(id)
);


CREATE TABLE Map(
    id BIGSERIAL,
    name TEXT,
    description TEXT,
    type MAPTYPE,
    storeId BIGINT,
    enemyId BIGINT,
    maxEnemies INTEGER,

    CONSTRAINT Map_PK PRIMARY KEY(id),
    CONSTRAINT Map_Enemy_FK FOREIGN KEY(enemyId) REFERENCES Enemy(id),
    CONSTRAINT Map_Store_FK FOREIGN KEY(storeId) REFERENCES Store(id)
);

CREATE TABLE Conecta(
    directionEnum DIRECTIONS,
    connectedFrom BIGINT,
    connectedTo BIGINT,

    CONSTRAINT Conecta_MapFrom_FK FOREIGN KEY(connectedFrom) REFERENCES Map(id),
    CONSTRAINT Conecta_MapTo_FK FOREIGN KEY(connectedTo) REFERENCES Map(id)
);


CREATE TABLE MapVolatile(
    id BIGINT NOT NULL UNIQUE,
    money UBIGINT,

    CONSTRAINT MapVolatile_Map_FK FOREIGN KEY(id) REFERENCES Map(id)
);

CREATE TABLE MapVolatileGuardaItem(
    itemId BIGINT,
    mapVolatileId BIGINT,

    CONSTRAINT MapVolatileGuardaItem_Item_FK FOREIGN KEY(itemId) REFERENCES Item(id),
    CONSTRAINT MapVolatileGuardaItem_MapVolatile_FK FOREIGN KEY(mapVolatileId) REFERENCES MapVolatile(id)    
);


CREATE TABLE EnemyInstance(
    id BIGINT,
    templateId BIGINT NOT NULL,
    hitPoints INTEGER,
    mapId BIGINT NOT NULL,
    nextAttackTime BIGINT,

    CONSTRAINT EnemyInstance_PK PRIMARY KEY(id),
    CONSTRAINT EnemyInstance_Enemy_FK FOREIGN KEY(templateId) REFERENCES Enemy(id),
    CONSTRAINT EnemyInstance_Map_FK FOREIGN KEY(mapId) REFERENCES Map(id)    
);


CREATE TABLE Player(
    id BIGINT,
    name TEXT UNIQUE NOT NULL,
    pass TEXT,
    rank PLAYERRANK,
    statPoints INTEGER,
    experience INTEGER,
    level INTEGER,
    mapId BIGINT NOT NULL,
    money UBIGINT,
    hitPoints INTEGER,
    nextAttackTime BIGINT,
    attributes ATTRIBUTE,
    weaponId BIGINT,
    armorId BIGINT,

    CONSTRAINT Player_PK PRIMARY KEY(id),
    CONSTRAINT Player_Weapon_FK FOREIGN KEY(weaponId) REFERENCES Item(id),
    CONSTRAINT Player_Armor_FK FOREIGN KEY(armorId) REFERENCES Item(id),    
    CONSTRAINT Player_Map_FK FOREIGN KEY(mapId) REFERENCES Map(id)
);

CREATE TABLE Inventory(
    playerId BIGINT,
    itemId BIGINT,

    CONSTRAINT Inventory_Player_FK FOREIGN KEY(playerId) REFERENCES Player(id),
    CONSTRAINT Inventory_Item_FK FOREIGN KEY(itemId) REFERENCES Item(id)
);
