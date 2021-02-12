# Editor de texto do João

Projeto feito para a matéria de Laboratório de Algorítmos
e técnicas de Programação II.

Inspirado fortemente no editor de texto clássico do sistema UNIX, **ed**.

## Manual
### Como funciona

```sh
$ ./jed                 -> começa a editar na memória
$ ./jed "arquivo.txt"   -> edita o arquivo "arquivo.txt"
```

### Comandos

* `:i num` --- escreve (sobreescreve) a linha num
* `:p num` --- printa a linha num

> `:p %` --- printa todas linhas

* `:d num`                  --- deleta a linha num
* `:f padrão`               --- procura e printa toda linha que tiver esse padrão
* `:s num padrão1 padrão2`  --- substitui na linha num o padrão1 pelo padrão2

> `:s % padrão1 padrão2`    --- substitui em todas linhas

* `:w "arquivo.txt"`        --- salva as o texto no arquivo `arquivo.txt`

> `:w` --- salva o texto no arquivo que ja foi aberto

* `:q` --- fecha o programa
