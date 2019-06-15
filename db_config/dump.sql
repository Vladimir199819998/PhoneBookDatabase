create table if not exists 'phone_book'(
    `id` int unsigned auto_increment primary key,
    `name` varchar(64) not null,
    `last_name` varchar(64) not null,
    `phone` varchar(20) not null)
    CHARACTER SET utf8;
