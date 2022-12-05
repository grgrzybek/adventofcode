/*
 * Copyright 2022 Grzegorz Grzybek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

use std::cmp::Ordering;
use std::io::{BufRead, BufReader, Error, ErrorKind};
use adventofcode2022::Options;

fn main() -> Result<(), Error> {
    let options = Options::solution("day01");
    println!("Starting {}", options.day());

    let mut answers = (0, 0);

    let mut calories : Vec<u32> = Vec::new();

    let file: &std::fs::File = options.file().ok_or(Error::from(ErrorKind::InvalidInput))?;

    let reader = BufReader::new(file);
    let mut single_elf = 0u32;
    reader.lines().enumerate().into_iter().filter(|l| { l.1.is_ok()}).map(|l| l.1.unwrap()).for_each(|line| {
        if line == "" {
            calories.push(single_elf);
            single_elf = 0;
        } else {
            single_elf += line.parse::<u32>().unwrap();
        }
    });
    calories.push(single_elf);

    calories.sort_unstable_by(|a, b| { if a > b { Ordering::Less } else { Ordering::Greater }});

    answers.0 = calories[0];
    answers.1 = calories[0] + calories[1] + calories[2];

    println!("Answer1: {}, Answer2: {}", answers.0, answers.1);

    Ok(())
}
