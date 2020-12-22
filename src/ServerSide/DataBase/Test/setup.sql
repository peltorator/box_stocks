drop table if exists Item;
create table Item (
    itemID integer primary key,
    itemName text, weight integer,
    volume integer,
    cost integer,
    amount integer,
    image text
);

drop table if exists Box;
create table Box (
    boxID integer primary key,
    boxName text,
    maxWeight integer,
    maxVolume integer,
    cost integer,
    available integer,
    image text
);

drop table if exists Users;
create table Users (
    userID integer primary key,
    userName text
);

drop table if exists Orders;
create table Orders (
    orderID integer primary key,
    userID integer,
    orderDate text,
    foreign key(userID) references Users(userID)
);

drop table if exists FilledBox;
create table FilledBox (
    filledBoxID integer primary key,
    boxID integer,
    orderID integer,
    foreign key(boxID) references Box(boxID),
    foreign key(orderID) references Orders(orderID)
);

drop table if exists ItemsForFilledBox;
create table ItemsForFilledBox (
    itemsForFilledBoxID integer primary key,
    itemID integer,
    filledBoxID integer,
    foreign key(itemID) references Items(itemID),
    foreign key(filledBoxID) references Orders(filledBoxID)
);
