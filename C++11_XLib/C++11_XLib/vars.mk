#这是一个make 调试辅助文件


%:
	@echo '$*=$($*)'
d-%:
	@echo '$*=$($*)'
	@echo 'origin=$(origin $*)'
	@echo 'value=$(value $*)'
	@echo 'flavor=$(flavor $*)'
