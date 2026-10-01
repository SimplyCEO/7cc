local append = field.create("append", { key = "value" })

field.add(0, append)

xml.close(0)

print(cc_output)

