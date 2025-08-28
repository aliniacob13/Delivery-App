# Delivery app (C++17 + MySQL)

Aplicație de tip consolă pentru un sistem de „food delivery”: restaurante, produse (mâncare, desert, băuturi), comenzi, staff (bucătari, casieri, livratori, supervisor), clienți și review-uri. Datele se memorează în **MySQL**, logica în **C++17**.

> **Pe scurt:** clienții văd restaurantele și meniurile, plasează comenzi; bucătarii le prepară; casierii/livratorii distribuie și livrează; clienții lasă review-uri; ratingul restaurantelor se actualizează; administratorul poate adăuga restaurante și aplica reduceri automate la cele slab cotate.

---

## Cuprins

- [Funcționalități](#funcționalități)
- [Arhitectură & Design Patterns](#arhitectură--design-patterns)
- [Cerințe](#cerințe)
- [Setup MySQL](#setup-mysql)
- [Build & Run](#build--run)


---

## Funcționalități

- **Client**
  - creare cont, login, resetare parolă
  - vizualizare restaurante + meniu structurat
  - plasare comandă (se salvează **snapshot** per item: nume, preț, kcal)
  - vizualizare comenzi livrate
  - review pentru comandă + livrator; recalcul rating restaurant

- **Staff**
  - **Bucătar**: coadă comenzi, marchează „preparată”
  - **Casier**: distribuie comenzi către bucătari și livratori
  - **Livrator**: livrează comenzi cu **prioritate** (Gold > Silver > Bronze > Basic; egalitate → FIFO)

- **Supervisor**
  - adaugă staff (bucătari/casieri/livratori)
  - adaugă produse (ID-urile se calculează local ca *max + 1*)

- **Administrator**
  - adaugă restaurante + supervisor
  - listă restaurante (nume + rating)
  - **reduceri** (ex: −20%) pentru restaurante cu rating ≤ 3

---

## Arhitectură & Design Patterns

| Pattern       | Unde / De ce |
|---------------|---------------|
| **Singleton** | `Aplicatie` (container în memorie); `Admin` (autentificare) |
| **Facade**    | `DatabaseFacade` – un singur punct de acces pentru MySQL |
| **Factory**   | `ProdusFactory` – creează `Mancare` / `Desert` / `Bautura` din rânduri DB |
| **Adapter**   | `RowAdapter<T>` + adaptoare concrete pentru maparea `MYSQL_ROW →` obiecte |
| **Strategy**  | `CategoryStrategy` (`ThreeCategoryStrategy`) + `CategoryContext` – calculează Basic/Bronze/Silver/Gold |


---

## Cerințe

- **C++17**
- **MySQL Server 8.x** (sau compatibil)
- **MySQL Connector/C** (`mysqlclient`)
- macOS (Homebrew) / Linux (`libmysqlclient-dev`) / Windows (MSVC + Connector/C)


---


## Setup MySQL
```
brew services start mysql
mysql -u root -p
```


---


## Build & Run
```
g++ -std=gnu++20 -O2 \
  -I/opt/homebrew/include \
  -I/opt/homebrew/include/mysql \
  main.cpp \
  -L/opt/homebrew/lib -lmysqlclient \
  -o app
```


---
