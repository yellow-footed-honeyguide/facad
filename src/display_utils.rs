use crate::file_entry::FileEntry;
use unicode_width::UnicodeWidthStr;

pub fn display_entries(entries: &[FileEntry], term_width: usize) {
    let max_columns = 4;
    let num_entries = entries.len();

    // Функция для вычисления ширины отображения элемента
    let entry_width = |entry: &FileEntry| -> usize {
        entry.emoji.width() + 1 + UnicodeWidthStr::width(entry.name.as_str())
    };

    // Находим оптимальное количество столбцов
    let (num_columns, column_widths) = (1..=max_columns)
        .rev() // Начинаем с максимального количества столбцов
        .find_map(|cols| {
            let rows = (num_entries + cols - 1) / cols;
            let widths: Vec<usize> = (0..cols)
                .map(|col| {
                    entries.iter()
                        .skip(col * rows)
                        .take(rows)
                        .map(entry_width)
                        .max()
                        .unwrap_or(0)
                })
                .collect();
            
            let total_width = widths.iter().sum::<usize>() + (cols - 1) * 2;
            if total_width <= term_width {
                Some((cols, widths))
            } else {
                None
            }
        })
        .unwrap_or((1, vec![entries.iter().map(entry_width).max().unwrap_or(0)]));

    let num_rows = (num_entries + num_columns - 1) / num_columns;

    for row in 0..num_rows {
        for col in 0..num_columns {
            let index = row + col * num_rows;
            if index < num_entries {
                let entry = &entries[index];
                let display_width = entry_width(entry);
                print!("{} {}", entry.emoji, entry.name);
                
                // Добавляем пробелы для выравнивания в пределах столбца
                if col < num_columns - 1 {
                    print!("{:width$}", "", width = column_widths[col] - display_width);
                    // Добавляем два пробела между столбцами
                    print!("  ");
                }
            }
        }
        println!();
    }
}
